#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"

#define MAX_EVENTS    1024
#define READ_BUFFER    1024

void setnonblocking(int fd) {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

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

    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error!");

    struct epoll_event events[MAX_EVENTS], ev;
    bzero(events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = servSockfd;
    ev.events = EPOLLIN | EPOLLET;
    setnonblocking(servSockfd);
    epoll_ctl(epfd, EPOLL_CTL_ADD, servSockfd, &ev);

    while(true) {
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
        errif(nfds == -1, "epoll wait error!");

        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == servSockfd) {
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                bzero(&clientAddr, clientAddrLen);

                int clientSockfd = accept(servSockfd, (sockaddr *)&clientAddr, &clientAddrLen);
                errif(clientSockfd == -1, "socket accept error!");

                std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr.sin_addr)
                        << ", Port : " << ntohs(clientAddr.sin_port) << ".\n";

                bzero(&ev, sizeof(ev));
                ev.data.fd = clientSockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clientSockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clientSockfd, &ev);
            } else if(events[i].events & EPOLLIN) {
                char buf[READ_BUFFER] = {0};
                while(true) {
                    bzero(buf, READ_BUFFER);
                    ssize_t readBytes = read(events[i].data.fd, buf, READ_BUFFER);
                    if(readBytes > 0) {
                        std::cout << "There has a message from client : " << events[i].data.fd << ", context is : " << buf << std::endl;
                        write(events[i].data.fd, buf, sizeof(buf));
                    } else if(readBytes == -1 && errno == EINTR) {
                        std::cout << "Continue reading." << std::endl;
                        continue;;
                    } else if(readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                        std::cout << "Finish reading once, errno is : " << errno << std::endl;
                        break;
                    } else if(readBytes == 0) {
                        std::cout << "EOF! client fd : " << events[i].data.fd << " disconnected!" << std::endl;
                        close(events[i].data.fd);
                        break;
                    }
                }
            } else {
                std::cout << "Something else happened!" << std::endl;
            }
        }
    }

    close(servSockfd);
    return 0;
}
