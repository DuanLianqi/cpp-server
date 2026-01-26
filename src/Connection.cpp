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
    channel->enableRead();
    channel->useET();

    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setReadCallback(cb);
    channel->setUseThreadPool(true);
    readBuffer = new Buffer();
}

Connection::~Connection() {
    delete channel;
    delete readBuffer;
}

void Connection::send(int sockfd) {
    char buf[READ_BUFFER] = {0};
    size_t dataSize = readBuffer->size();

    for(int i = 0; i < dataSize; i++) {
        buf[i] = readBuffer->c_str()[i];
    }

    size_t dataLeft = dataSize;
    while(dataLeft > 0) {
        ssize_t writeBytes = write(sockfd, buf + dataSize - dataLeft, dataLeft);
        if(writeBytes == -1 && errno == EAGAIN) {
            break;
        }
        dataLeft -= writeBytes;
    }
}

void Connection::echo(int sockfd) {
    char buf[READ_BUFFER] = {0};
    while(true) {
        memset(buf, 0, READ_BUFFER);
        ssize_t readBytes = read(sockfd, buf, READ_BUFFER);
        if(readBytes > 0) {
            readBuffer->append(buf, readBytes);
        } else if(readBytes == -1 && errno == EINTR) {
            std::cout << "Continue reading." << std::endl;
            continue;
        } else if(readBytes == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))) {
            std::cout << "Finish reading once!" << std::endl;
            std::cout << "There has a message from client : " << sockfd << ", context is : " << readBuffer->c_str() << std::endl;
            send(sockfd);
            readBuffer->clear();
            break;
        } else if(readBytes == 0) {
            std::cout << "EOF! client fd : " << sockfd << " disconnected!" << std::endl;
            deleteConnectionCallback(sockfd);
            break;
        } else {
            std::cout << "Connection reset by peer! " << std::endl;
            deleteConnectionCallback(sockfd);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(int)> _cb) {
    deleteConnectionCallback = _cb;
}
