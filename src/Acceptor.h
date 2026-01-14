#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

class Socket;
class Channel;
class EventLoop;

class Acceptor {
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    void setNewConnectionCallback(std::function<void(Socket *)> _cb);
private:
    Socket *sock;
    Channel *acceptChannel;
    EventLoop *loop;
    std::function<void(Socket *)> newConnectionCallback;
};

#endif
