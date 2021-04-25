//
// Created by 水瀬あくあ on 2021/4/25.
//

#include "Channel.h"
#include "IChannelCallBack.h"
#include <sys/epoll.h>
#include <iostream>

Channel::Channel(int epollfd, int sockfd):
    _epollfd(epollfd),
    _sockfd(sockfd),
    _events(0),
    _revents(0),
    _callBack(nullptr) {}

Channel::~Channel() = default;

void Channel::setCallBack(IChannelCallBack* callBack) {
    _callBack = callBack;
}

void Channel::setRevents(int revents) {
    _revents = revents;
}

void Channel::handleEvent() {
    if(_revents & EPOLLIN) {
        _callBack->OnIn(_sockfd);
    }
}

int Channel::getSockfd() {
    return _sockfd;
}

void Channel::enableReading() {
    _events |= EPOLLIN;
    update();
}

void Channel::update() {
    struct epoll_event ev;
    ev.data.ptr = this; //epoll_data记录此Channel的指针
    ev.events = _events;
    if(-1 == epoll_ctl(_epollfd, EPOLL_CTL_ADD, _sockfd, &ev)) {
        std::cout << "epoll add new event error, errno: " << errno <<  std::endl;
    }
}