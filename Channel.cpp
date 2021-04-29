//
// Created by 水瀬あくあ on 2021/4/25.
//

#include "Channel.h"
#include "IChannelCallBack.h"
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(EventLoop* loop, int sockfd):
    _loop(loop),
    _sockfd(sockfd),
    _events(0),
    _revents(0),
    _pCallBack(nullptr) {}

Channel::~Channel() = default;

void Channel::setCallBack(IChannelCallBack* callBack) {
    _pCallBack = callBack;
}

void Channel::setRevents(int revents) {
    _revents = revents;
}

void Channel::handleEvent() {
    if(_revents & EPOLLIN) {
        _pCallBack->handleRead();
    }
    if(_revents & EPOLLOUT) {
        _pCallBack->handleWrite();
    }
}

int Channel::getEvents() {
    return _events;
}

int Channel::getSockfd() {
    return _sockfd;
}

void Channel::enableReading() {
    _events |= EPOLLIN;
    update();
}

void Channel::enableWriting() {
    _events |= EPOLLOUT;
    update();
}

void Channel::disableWriting() {
    _events &= ~EPOLLOUT;
    update();
}

bool Channel::isWriting() {
    return _events & EPOLLOUT;
}

void Channel::update() {
    _loop->update(this);
}