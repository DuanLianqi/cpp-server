#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>

class Epoll;

class Channel {
public:
    Channel(Epoll *_ep, int _fd);
    ~Channel();

    void enableReading();
    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    void setInEpoll();
    void setEvents(uint32_t _events);
    void setRevents(uint32_t _revents);

private:
    Epoll *ep;
    int fd;
    uint32_t events;    //希望监听这个文件描述符的哪些事件
    uint32_t revents;   //在epoll返回该Channel时文件描述符正在发生的事件
    bool inEpoll;
};

#endif
