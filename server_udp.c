#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080           // 서버가 사용할 포트
#define BUFFER_SIZE 1024    // 버퍼 크기

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    // 1. 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }


    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // 모든 네트워크 인터페이스 허용
    server_addr.sin_port = htons(PORT);

    // 3. 소켓 바인딩
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("바인딩 실패");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("UDP 서버가 실행 중입니다. 클라이언트의 메시지를 기다립니다...\n");

    // 4. 데이터 수신 및 송신
    while (1) {
        int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                                (struct sockaddr *)&client_addr, &addr_len);
        if (recv_len > 0) {
            buffer[recv_len] = '\0';  // 문자열 끝에 NULL 추가
            printf("클라이언트로부터 수신: %s\n", buffer);

            // 클라이언트로 응답
            sendto(sockfd, buffer, recv_len, 0,
                   (struct sockaddr *)&client_addr, addr_len);
            printf("클라이언트로 응답: %s\n", buffer);
        }
    }

    close(sockfd);
    return 0;
}
