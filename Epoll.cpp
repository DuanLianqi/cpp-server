#include <unistd.h>
#include <cstring>
#include "Channel.h"
#include "Epoll.h"
#include "util.h"

#define MAX_EVENTS    1024

Epoll::Epoll() : epfd(-1) {
    epfd = epoll_create1(0);
    errif(epfd == - 1, "epoll create error!");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if(epfd != -1) {
        close(epfd);
        epfd = -1;
    }
    delete []events;
}

void Epoll::addFd(int fd, uint32_t op) {
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;

    int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
    errif(ret == -1, "epoll add event error!");
}

void Epoll::updateChannel(Channel *ch) {
    int fd = ch->getFd();

    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = ch;
    ev.events = ch->getEvents();

    if(!ch->getInEpoll()) {
        int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
        errif(ret == -1, "epoll add event error!");
        ch->setInEpoll();
    } else {
        int ret = epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
        errif(ret == -1, "epoll modidfy event error!");
    }
}

std::vector<Channel *> Epoll::poll(int timeout) {
    std::vector<Channel *> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(nfds == -1, "epoll wait error!");

    for(int i = 0; i < nfds; i++) {
        Channel *ch = (Channel*)(events[i].data.ptr);
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}
