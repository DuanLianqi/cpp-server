#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>

class Socket;
class Channel;
class EventLoop;

class Connection {
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket *)> _cb);

private:
    Socket *sock;
    Channel *channel;
    EventLoop *loop;
    std::function<void(Socket *)> deleteConnectionCallback;
};

#endif
