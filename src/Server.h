#ifndef SERVER_H
#define SERVER_H

#include <map>

class Socket;
class EventLoop;
class Acceptor;
class Connection;

class Server {
public:
    Server(EventLoop *_loop);
    ~Server();
    void newConnection(Socket *servSocket);
    void deleteConnection(Socket *servSocket);

private:
    EventLoop *loop;
    Acceptor *acceptor;
    std::map<int, Connection *> connections;
};

#endif
