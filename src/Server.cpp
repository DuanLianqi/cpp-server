#include <functional>
#include "Socket.h"
#include "Acceptor.h"
#include "Connection.h"
#include "Server.h"

Server::Server(EventLoop *_loop) : loop(_loop), acceptor(nullptr) {
    acceptor = new Acceptor(loop);
    std::function<void(Socket *)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);
}

Server::~Server() {
    delete acceptor;
}

void Server::newConnection(Socket *servSocket) {
    if(servSocket->getFd() != -1) {
        Connection *newConnection = new Connection(loop, servSocket);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        newConnection->setDeleteConnectionCallback(cb);
        connections[servSocket->getFd()] = newConnection;
    }
}

void Server::deleteConnection(int sockfd) {
    if(sockfd != -1) {
        auto it = connections.find(sockfd);
        if(it != connections.end()) {
            Connection *connection = connections[sockfd];
            connections.erase(sockfd);
            delete connection;
        }
    }
}
