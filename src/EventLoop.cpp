#include <vector>
#include "Epoll.h"
#include "Channel.h"
#include "EventLoop.h"
#include "ThreadPool.h"

EventLoop::EventLoop() : ep(nullptr), threadPool(nullptr), quit(false) {
    ep = new Epoll();
    threadPool = new ThreadPool();
}

EventLoop::~EventLoop() {
    delete threadPool;
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

void EventLoop::addThread(std::function<void()> func) {
    threadPool->add(func);
}
