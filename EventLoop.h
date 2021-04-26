//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_EVENTLOOP_H
#define CPP_MUDUO_LEARNING_EVENTLOOP_H

#include "Epoll.h"
#include "Channel.h"

struct EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* channel);
private:
    bool _quit;
    Epoll* _poller;
};

#endif //CPP_MUDUO_LEARNING_EVENTLOOP_H
