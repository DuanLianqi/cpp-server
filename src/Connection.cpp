#include <iostream>
#include <unistd.h>
#include <cstring>
#include "Socket.h"
#include "Channel.h"
#include "Connection.h"

#define READ_BUFFER    1024

Connection::Connection(EventLoop *_loop, Socket *_sock) : loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
}

void Connection::echo(int sockfd) {
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
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = _cb;
}
