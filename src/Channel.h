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
    void enableRead();

    int getFd() const;
    uint32_t getEvents() const;
    uint32_t getReady() const;
    bool getInEpoll() const;
    void setInEpoll(bool _in = true);
    void setEvents(uint32_t _events);
    void setReady(uint32_t _ready);
    void setReadCallback(std::function<void()> cb);
    void setWriteCallback(std::function<void()> cb);
    void useET();
    void setUseThreadPool(bool use = true);

private:
    EventLoop *loop;
    int fd;
    uint32_t events;    //希望监听这个文件描述符的哪些事件
    uint32_t ready;
    bool useThreadPool;
    bool inEpoll;
    std::function<void()> readCallback;
    std::function<void()> writeCallback;
};

#endif
