#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include "Socket.h"
#include "InetAddress.h"
#include "util.h"

Socket::Socket() : fd(-1) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(fd == -1, "socket create error!");
}

Socket::Socket(int _fd) : fd(_fd) {
    errif(fd == -1, "socket create error!");
}

Socket::~Socket() {
    if(fd != -1) {
        close(fd);
    }
    fd = -1;
}

void Socket::bind(InetAddress *addr) {
    struct sockaddr_in sockAddr = addr->getAddr();
    int ret = ::bind(fd, (sockaddr *)&(sockAddr), sizeof(sockAddr));
    errif(ret == -1, "socket bind error!");
}

void Socket::listen() {
    int ret = ::listen(fd, SOMAXCONN);
    errif(ret == -1, "socket listen error!");
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *addr) {
    struct sockaddr_in sockAddr;
    socklen_t addrLen = sizeof(sockAddr);
    memset(&sockAddr, 0, sizeof(sockAddr));
    int clientSockfd = ::accept(fd, (sockaddr *)&(sockAddr), &addrLen);
    errif(clientSockfd == -1, "socket accept error!");
    addr->setInetAddr(sockAddr);
    return clientSockfd;
}

void Socket::connect(InetAddress *addr) {
    struct sockaddr_in sockAddr = addr->getAddr();
    int ret = ::connect(fd, (sockaddr *)&(sockAddr), sizeof(sockAddr));
    errif(ret == -1, "socket connect error!");
}

int Socket::getFd() const {
    return fd;
}
