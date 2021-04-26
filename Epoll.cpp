//
// Created by 水瀬あくあ on 2021/4/26.
//

#include "Epoll.h"
#include <iostream>

Epoll::Epoll() {
    //创建epoll
    _epollfd = epoll_create(1);
    if(_epollfd <= 0) {
        std::cout << "epoll error, error:" << _epollfd << std::endl;
        exit(EPOLL_ERROR);
    }
}

Epoll::~Epoll() = default;

void Epoll::poll(std::vector<Channel *>& channels) {
    int fds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1); //epoll监听到有事件
    if(fds == -1) {
        std::cout << "epoll wait error， errno:" << errno << std::endl;
        return;
    }
    for(int i = 0; i < fds; i++) {
        auto* pChannel = static_cast<Channel*>(_events[i].data.ptr);
        pChannel->setRevents(_events[i].events);
        channels.push_back(pChannel);
    }
}

void Epoll::update(Channel *channel) {
    struct epoll_event ev;
    ev.data.ptr = channel; //epoll_data记录此Channel的指针
    ev.events = channel->getEvents();
    int sockfd = channel->getSockfd();
    if(-1 == epoll_ctl(_epollfd, EPOLL_CTL_ADD, sockfd, &ev)) {
        std::cout << "epoll add new event error, errno: " << errno << std::endl;
    }
}