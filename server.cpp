#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

int main() {
    int servSockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(12345);

    bind(servSockfd, (sockaddr *)&servAddr, sizeof(servAddr));

    listen(servSockfd, SOMAXCONN);

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    bzero(&clientAddr, clientAddrLen);

    int clientSockfd = accept(servSockfd, (sockaddr *)&clientAddr, &clientAddrLen);

    std::cout << "new client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr.sin_addr)
              << ", Port : " << ntohs(clientAddr.sin_port) << ".\n";

    return 0;
}
