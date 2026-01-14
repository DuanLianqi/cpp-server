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
    Connection *newConnection = new Connection(loop, servSocket);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    newConnection->setDeleteConnectionCallback(cb);
    connections[servSocket->getFd()] = newConnection;
}

void Server::deleteConnection(Socket *servSocket) {
    Connection *deleteConnection = connections[servSocket->getFd()];
    connections.erase(servSocket->getFd());
    delete deleteConnection;
}
