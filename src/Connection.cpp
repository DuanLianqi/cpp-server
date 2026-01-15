#include <iostream>
#include <unistd.h>
#include <cstring>
#include "util.h"
#include "Socket.h"
#include "Channel.h"
#include "Buffer.h"
#include "Connection.h"

#define READ_BUFFER    1024

Connection::Connection(EventLoop *_loop, Socket *_sock) \
    : loop(_loop), sock(_sock), channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb);
    channel->enableReading();
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete readBuffer;
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER] = {0};
    while(true) {
        bzero(buf, READ_BUFFER);
        ssize_t readBytes = read(sockfd, buf, READ_BUFFER);
        if(readBytes > 0) {
            readBuffer->append(buf, readBytes);
        } else if(readBytes == -1 && errno == EINTR) {
            std::cout << "Continue reading." << std::endl;
            continue;
        } else if(readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "Finish reading once!" << std::endl;
            std::cout << "There has a message from client : " << sockfd << ", context is : " << readBuffer->c_str() << std::endl;
            int ret = write(sockfd, readBuffer->c_str(), readBuffer->size());
            errif(ret == -1, "Server write message error!");
            readBuffer->clear();
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
