//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_EVENTLOOP_H
#define CPP_MUDUO_LEARNING_EVENTLOOP_H

#include <vector>

#include "Epoll.h"
#include "Channel.h"
#include "IRun.h"
#include "IChannelCallBack.h"

struct EventLoop: public IChannelCallBack {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* channel);
    void queueLoop(IRun* pRun);
    virtual void handleRead();
    virtual void handleWrite();

private:
    void wakeup();
    int createEventfd();
    void doPendingFuctors();

    bool _quit;
    Epoll* _poller;
    int _eventfd;
    Channel* _wakeupChannel;
    std::vector<IRun*> _pendingFuctors;
};

#endif //CPP_MUDUO_LEARNING_EVENTLOOP_H
