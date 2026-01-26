#ifndef CONNECTION_H
#define CONNECTION_H

#include <functional>
#include <string>

class Socket;
class Channel;
class EventLoop;
class Buffer;

class Connection {
public:
    Connection(EventLoop *_loop, Socket *_sock);
    ~Connection();

    void send(int sockfd);
    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(int)> _cb);

private:
    Socket *sock;
    Channel *channel;
    EventLoop *loop;
    std::string *inBuffer;
    Buffer *readBuffer;
    std::function<void(int)> deleteConnectionCallback;
};

#endif
