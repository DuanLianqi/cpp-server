#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

int main() {
    int servSockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(servSockfd == - 1, "socket create error!");

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(12345);

    int ret = bind(servSockfd, (sockaddr *)&servAddr, sizeof(servAddr));
    errif(ret == -1, "socket bind error!");

    ret = listen(servSockfd, SOMAXCONN);
    errif(ret == -1, "socket listen error!");

    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    bzero(&clientAddr, clientAddrLen);

    int clientSockfd = accept(servSockfd, (sockaddr *)&clientAddr, &clientAddrLen);
    errif(clientSockfd == -1, "socket accept error!");

    std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr.sin_addr)
              << ", Port : " << ntohs(clientAddr.sin_port) << ".\n";

    while(true) {
        char buf[1024] = {0};
        ssize_t readBytes = read(clientSockfd, buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "There has a message from client : " << clientSockfd << ", context is : " << buf << std::endl;
            write(clientSockfd, buf, sizeof(buf));
        } else if(readBytes == 0) {
            std::cout << "Client disconnected, fd : " << clientSockfd << std::endl;
            close(clientSockfd);
            break;
        } else if(readBytes == -1) {
            close(clientSockfd);
            errif(true, "socket read error!");
            break;
        }
    }

    close(servSockfd);
    return 0;
}
