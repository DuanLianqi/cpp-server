#include <iostream>
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include "Acceptor.h"

Acceptor::Acceptor(EventLoop *_loop) : loop(_loop), sock(nullptr), acceptChannel(nullptr) {
    sock = new Socket();
    InetAddress *addr = new InetAddress("127.0.0.1", 12345);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();

    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();

    delete addr;
}

Acceptor::~Acceptor() {
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection() {
    InetAddress *clientAddr = new InetAddress();
    int clientSockfd = sock->accept(clientAddr);
    Socket *clientSocket = new Socket(clientSockfd);

    std::cout << "New client fd : " << clientSockfd << ", IP : " << inet_ntoa(clientAddr->addr.sin_addr)
            << ", Port : " << ntohs(clientAddr->addr.sin_port) << ".\n";

    clientSocket->setnonblocking();
    newConnectionCallback(clientSocket);
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket *)> _cb) {
    newConnectionCallback = _cb;
}
