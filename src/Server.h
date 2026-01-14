#ifndef SERVER_H
#define SERVER_H

class Socket;
class EventLoop;
class Acceptor;

class Server {
public:
    Server(EventLoop *_loop);
    ~Server();
    void handleReadEvent(int sockfd);
    void newConnection(Socket *servSocket);

private:
    EventLoop *loop;
    Acceptor *acceptor;
};

#endif
