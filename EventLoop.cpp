//
// Created by 水瀬あくあ on 2021/4/26.
//

#include <sys/eventfd.h>
#include "EventLoop.h"
#include <unistd.h>
#include <iostream>

EventLoop::EventLoop():
    _quit(false),
    _poller(new Epoll()) {
    _eventfd = createEventfd();
    _wakeupChannel = new Channel(this, _eventfd);
    _wakeupChannel->setCallBack(this);
    _wakeupChannel->enableReading();
}

EventLoop::~EventLoop() {
    delete _poller;
}

void EventLoop::loop() {
    while(!_quit) {
        std::vector<Channel*> channels;
        _poller->poll(channels);
        for(auto c : channels) {
            c->handleEvent();
        }
        doPendingFuctors();
    }
}

void EventLoop::update(Channel *channel) {
    _poller->update(channel);
}

void EventLoop::handleRead() {
    uint64_t one = 1;
    ssize_t n = read(_eventfd, &one, sizeof one);
    if(n != sizeof one) {
        std::cout << "EventEventLoop::handleRead() reads " << n << " bytes instead of 8" << std::endl;
    }
}

void EventLoop::handleWrite() {

}

void EventLoop::queueLoop(IRun *pRun) {
    _pendingFuctors.push_back(pRun);
    wakeup();
}

void EventLoop::wakeup() {
    uint64_t one = 1;
    ssize_t n = write(_eventfd, &one, sizeof one);
    if(n != sizeof one) {
        std::cout << "EventLoop::wakeup() writes " << n << " bytes instead of 8" << std::endl;
    }
}

int EventLoop::createEventfd() {
    int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    if(evtfd < 0) {
        std::cout << "Failed in eventfd" << std::endl;
    }
    return evtfd;
}

void EventLoop::doPendingFuctors() {
    std::vector<IRun*> tempRuns;
    tempRuns.swap(_pendingFuctors);
    for(auto i : tempRuns) {
        i->run();
    }
}