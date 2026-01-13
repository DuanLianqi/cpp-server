#include <iostream>
#include <unistd.h>
#include <cstring>
#include <functional>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Server.h"

#define READ_BUFFER    1024

Server::Server(EventLoop *_loop) : loop(_loop) {
    Socket *servSocket = new Socket();
    InetAddress *servAddr = new InetAddress("127.0.0.1", 12345);
    servSocket->bind(servAddr);
    servSocket->listen();
    servSocket->setnonblocking();

    Channel *servChannel = new Channel(loop, servSocket->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, servSocket);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server() {

}

void Server::handleReadEvent(int sockfd) {
    char buf[READ_BUFFER] = {0};
    while(true) {
        bzero(buf, READ_BUFFER);
        ssize_t readBytes = read(sockfd, buf, READ_BUFFER);
        if(readBytes > 0) {
            std::cout << "There has a message from client : " << sockfd << ", context is : " << buf << std::endl;
            write(sockfd, buf, sizeof(buf));
        } else if(readBytes == -1 && errno == EINTR) {
            std::cout << "Continue reading." << std::endl;
            continue;
        } else if(readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "Finish reading once, errno is : " << errno << std::endl;
            break;
        } else if(readBytes == 0) {
            std::cout << "EOF! client fd : " << sockfd << " disconnected!" << std::endl;
            close(sockfd);
            break;
        }
    }
}

void Server::newConnection(Socket *servSocket) {
    InetAddress *clientAddr = new InetAddress();
    int clientSockfd = servSocket->accept(clientAddr);
    Socket *clientSocket = new Socket(clientSockfd);

    std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr->addr.sin_addr)
            << ", Port : " << ntohs(clientAddr->addr.sin_port) << ".\n";

    clientSocket->setnonblocking();
    Channel *clientChannel = new Channel(loop, clientSockfd);
    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clientSockfd);
    clientChannel->setCallback(cb);
    clientChannel->enableReading();
}
