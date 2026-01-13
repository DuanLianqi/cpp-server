#ifndef SERVER_H
#define SERVER_H

class EventLoop;
class Socket;

class Server {
public:
    Server(EventLoop *_loop);
    ~Server();
    void handleReadEvent(int sockfd);
    void newConnection(Socket *servSocket);

private:
    EventLoop *loop;
};

#endif
