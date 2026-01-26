#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include <functional>

class Epoll;
class Channel;
class ThreadPool;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *ch);
    void addThread(std::function<void()> func);
private:
    Epoll *ep;
    ThreadPool * threadPool;
    bool quit;
};

#endif
