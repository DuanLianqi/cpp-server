#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include "util.h"

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == - 1, "socket create error!");

    struct sockaddr_in servAddr;
    bzero(&servAddr, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servAddr.sin_port = htons(12345);

    int ret = connect(sockfd, (sockaddr *)&servAddr, sizeof(servAddr));
    errif(ret == -1, "socket connect error!");

    while(true) {
        char buf[1024] = {0};
        std::cin >> buf;
        ssize_t writeBytes = write(sockfd, buf, sizeof(buf));
        if(writeBytes == -1) {
            std::cout << "Server has already disconnected, can't write any more." << std::endl;
            break;
        }

        bzero(buf, sizeof(buf));
        ssize_t readBytes = read(sockfd, buf, sizeof(buf));
        if(readBytes > 0) {
            std::cout << "There has a message from server : " << sockfd << ", context is : " << buf << std::endl;
        } else if(readBytes == 0) {
            std::cout << "Server has already disconnected." << std::endl;
            break;
        } else if(readBytes == -1) {
            errif(true, "socket read error!");
            break;
        }
    }

    close(sockfd);
    return 0;
}
