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
#include "TimeStamp.h"
#include "TimerQueue.h"

struct EventLoop: public IChannelCallBack {
public:
    class Runner{
    public:
        Runner(IRun* r, void* param):
            _pRun(r),
            _param(param) {};
        void doRun() {
            _pRun->run(_param);
        }
    private:
        IRun* _pRun;
        void* _param;
    };
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* channel);
    void queueLoop(IRun* pRun, void* param);
    int64_t runAt(TimeStamp when, IRun* pRun);
    int64_t runAfter(double delay, IRun* pRun);
    int64_t runEvery(double interval, IRun* pRun);
    void cancelTimer(int64_t timerfd);

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
    std::vector<Runner> _pendingFuctors;
    TimerQueue* _pTimerQueue;
};

#endif //CPP_MUDUO_LEARNING_EVENTLOOP_H
