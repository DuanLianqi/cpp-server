#ifndef EVENTLOOP_H
#define EVENTLOOP_H

class Epoll;
class Channel;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();

    void loop();
    void updateChannel(Channel *ch);
private:
    Epoll *ep;
    bool quit;
};

#endif
