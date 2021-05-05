//
// Created by 水瀬あくあ on 2021/4/26.
//

#include <sys/eventfd.h>
#include "EventLoop.h"
#include <unistd.h>
#include <iostream>

EventLoop::EventLoop():
    _quit(false),
    _poller(new Epoll()),
    _pTimerQueue(new TimerQueue(this)) {
    _eventfd = createEventfd();
    _wakeupChannel = new Channel(this, _eventfd);
    _wakeupChannel->setCallBack(this);
    _wakeupChannel->enableReading();
}

EventLoop::~EventLoop() {
    delete _poller;
    delete _pTimerQueue;
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

void EventLoop::queueLoop(IRun *pRun, void* param) {
    Runner r(pRun, param);
    _pendingFuctors.push_back(r);
    wakeup();
}

int64_t EventLoop::runAt(TimeStamp when, IRun *pRun) {
    return _pTimerQueue->addTimer(pRun, when, 0.0);
}

int64_t EventLoop::runAfter(double delay, IRun *pRun) {
    return _pTimerQueue->addTimer(pRun, TimeStamp::nowAfter(delay), 0.0);
}

int64_t EventLoop::runEvery(double interval, IRun *pRun) {
    return _pTimerQueue->addTimer(pRun, TimeStamp::nowAfter(interval), interval);
}

void EventLoop::cancelTimer(int64_t timerfd) {
    _pTimerQueue->cancelTimer(timerfd);
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
    std::vector<Runner> tempRuns;
    tempRuns.swap(_pendingFuctors);
    for(auto it : tempRuns) {
        it.doRun();
    }
}