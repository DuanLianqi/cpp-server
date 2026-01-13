#ifndef CHANNEL_H
#define CHANNEL_H

#include <sys/epoll.h>
#include <functional>

class EventLoop;

class Channel {
public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getRevents() const;
    bool getInEpoll() const;
    void setInEpoll();
    void setEvents(uint32_t _events);
    void setRevents(uint32_t _revents);
    void setCallback(std::function<void()> cb);

private:
    EventLoop *loop;
    int fd;
    uint32_t events;    //希望监听这个文件描述符的哪些事件
    uint32_t revents;   //在epoll返回该Channel时文件描述符正在发生的事件
    bool inEpoll;
    std::function<void()> callback;
};

#endif
