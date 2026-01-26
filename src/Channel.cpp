#include <unistd.h>
#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd) : loop(_loop), fd(_fd), events(0), 
    ready(0), useThreadPool(true), inEpoll(false) {

}

Channel::~Channel() {
    if(fd != -1) {
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent() {
    if(ready & (EPOLLIN | EPOLLPRI)) {
        if(useThreadPool) {
            loop->addThread(readCallback);
        } else {
            readCallback();
        }
    }

    if(ready & EPOLLOUT) {
        if(useThreadPool) {
            loop->addThread(writeCallback);
        } else {
            writeCallback();
        }
    }
}

void Channel::enableRead() {
    events |= EPOLLIN | EPOLLPRI;
    loop->updateChannel(this);
}

int Channel::getFd() const {
    return fd;
}

uint32_t Channel::getEvents() const {
    return events;
}

uint32_t Channel::getReady() const {
    return ready;
}

bool Channel::getInEpoll() const {
    return inEpoll;
}

void Channel::setInEpoll(bool _in) {
    inEpoll = _in;
}

void Channel::setEvents(uint32_t _events) {
    events = _events;
}

void Channel::setReady(uint32_t _ready) {
    ready = _ready;
}

void Channel::setReadCallback(std::function<void()> cb) {
    readCallback = cb;
}

void Channel::setWriteCallback(std::function<void()> cb) {
    writeCallback = cb;
}

void Channel::useET() {
    events |= EPOLLET;
    loop->updateChannel(this);
}

void Channel::setUseThreadPool(bool use) {
    useThreadPool = use;
}
