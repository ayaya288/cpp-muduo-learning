//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_EPOLL_H
#define CPP_MUDUO_LEARNING_EPOLL_H

#include "Define.h"
#include "Channel.h"
#include <sys/epoll.h>
#include <vector>

class Epoll {
public:
    Epoll();
    ~Epoll();

    void poll(std::vector<Channel*>& channels);
    void update(Channel* channel);

private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
};

#endif //CPP_MUDUO_LEARNING_EPOLL_H
