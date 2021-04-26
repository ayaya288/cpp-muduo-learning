//
// Created by 水瀬あくあ on 2021/4/26.
//

#include "EventLoop.h"

EventLoop::EventLoop():
    _quit(false),
    _poller(new Epoll()) {} //TODO:内存泄漏

EventLoop::~EventLoop() = default;

void EventLoop::loop() {
    while(!_quit) {
        std::vector<Channel*> channels;
        _poller->poll(channels);
        for(auto c : channels) {
            c->handleEvent();
        }
    }
}

void EventLoop::update(Channel *channel) {
    _poller->update(channel);
}
