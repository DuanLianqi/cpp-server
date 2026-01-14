#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <functional>

class Socket;
class InetAddress;
class Channel;
class EventLoop;

class Acceptor {
public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket *)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket *)> _cb);
private:
    Socket *sock;
    InetAddress *addr;
    Channel *acceptChannel;
    EventLoop *loop;
};

#endif
