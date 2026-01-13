#include <iostream>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
#include "util.h"
#include "InetAddress.h"
#include "Socket.h"
#include "Epoll.h"
#include "Channel.h"

#define READ_BUFFER 1024

int main() {
    Socket *servSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 12345);
    servSocket->bind(servAddr);
    servSocket->listen();
    servSocket->setnonblocking();

    Epoll *ep = new Epoll();
    Channel *servChannel = new Channel(ep, servSocket->getFd());
    servChannel->enableReading();

    while(true) {
        std::vector<Channel *> activeChannels = ep->poll();
        int nfds = activeChannels.size();

        for(int i = 0; i < nfds; i++) {
            int chfd = activeChannels[i]->getFd();
            if(chfd == servSocket->getFd()) {
                InetAddress *clientAddr = new InetAddress();
                int clientSockfd = servSocket->accept(clientAddr);
                Socket *clientSock = new Socket(clientSockfd);

                std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr->addr.sin_addr)
                        << ", Port : " << ntohs(clientAddr->addr.sin_port) << ".\n";

                clientSock->setnonblocking();
                Channel *clientChannel = new Channel(ep, clientSockfd);
                clientChannel->enableReading();
            } else if(activeChannels[i]->getRevents() & EPOLLIN) {
                char buf[READ_BUFFER] = {0};
                while(true) {
                    bzero(buf, READ_BUFFER);
                    ssize_t readBytes = read(chfd, buf, READ_BUFFER);
                    if(readBytes > 0) {
                        std::cout << "There has a message from client : " << chfd << ", context is : " << buf << std::endl;
                        write(chfd, buf, sizeof(buf));
                    } else if(readBytes == -1 && errno == EINTR) {
                        std::cout << "Continue reading." << std::endl;
                        continue;;
                    } else if(readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
                        std::cout << "Finish reading once, errno is : " << errno << std::endl;
                        break;
                    } else if(readBytes == 0) {
                        std::cout << "EOF! client fd : " << chfd << " disconnected!" << std::endl;
                        close(chfd);
                        break;
                    }
                }
            } else {
                std::cout << "Something else happened!" << std::endl;
            }
        }
    }

    delete servSocket;
    delete servAddr;
    delete ep;
    delete servChannel;
    return 0;
}
