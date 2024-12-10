

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080           // 서버 포트와 동일하게 설정
#define BUFFER_SIZE 1024    // 버퍼 크기

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char *message = "Hello, Server!";
    socklen_t addr_len = sizeof(server_addr);

    // 1. 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 2. 서버 주소 설정
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("잘못된 주소");
        close(sockfd);
        exit(EXIT_FAILURE);
    }


    // 3. 서버로 메시지 전송
    sendto(sockfd, message, strlen(message), 0,
           (struct sockaddr *)&server_addr, addr_len);
    printf("서버로 전송: %s\n", message);

    // 4. 서버 응답 수신
    int recv_len = recvfrom(sockfd, buffer, BUFFER_SIZE, 0,
                            (struct sockaddr *)&server_addr, &addr_len);
    if (recv_len > 0) {
        buffer[recv_len] = '\0';  // 문자열 끝에 NULL 추가
        printf("서버 응답: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}

