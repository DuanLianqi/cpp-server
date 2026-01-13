#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll {
public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op);
    void updateChannel(Channel *ch);
    std::vector<Channel *> poll(int timeout = -1);
    //std::vector<epoll_event> poll(int timeout = -1);

private:
    int epfd;
    struct epoll_event *events;
};

#endif
