#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"

#define READ_BUFFER 1024

int main() {
    Socket *servSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 12345);
    servSocket->bind(servAddr);
    servSocket->listen();
    servSocket->setnonblocking();

    Epoll *ep = new Epoll();
    ep->addFd(servSocket->getFd(), EPOLLIN | EPOLLET);

    while(true) {
        std::vector<epoll_event> events = ep->poll();
        int nfds = events.size();

        for(int i = 0; i < nfds; i++) {
            if(events[i].data.fd == servSocket->getFd()) {
                InetAddress *clientAddr = new InetAddress();
                int clientSockfd = servSocket->accept(clientAddr);
                Socket *clientSock = new Socket(clientSockfd);

                std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr->addr.sin_addr)
                        << ", Port : " << ntohs(clientAddr->addr.sin_port) << ".\n";

                clientSock->setnonblocking();
                ep->addFd(clientSockfd, EPOLLIN | EPOLLET);
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

    delete servSocket;
    delete ep;
    return 0;
}
