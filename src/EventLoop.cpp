#include <vector>
#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"

EventLoop::EventLoop() : ep(nullptr), quit(false) {
    ep = new Epoll();
}

EventLoop::~EventLoop() {
    delete ep;
}

void EventLoop::loop() {
    while(!quit) {
        std::vector<Channel *> activeChannels;
        activeChannels = ep->poll();
        for(auto& it : activeChannels) {
            it->handleEvent();
        }
    }
}

void EventLoop::updateChannel(Channel *ch) {
    ep->updateChannel(ch);
}
