#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(12345);

    connect(sockfd, (sockaddr *)&servAddr, sizeof(servAddr));

    return 0;
}
