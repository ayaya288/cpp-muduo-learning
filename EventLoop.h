//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_EVENTLOOP_H
#define CPP_MUDUO_LEARNING_EVENTLOOP_H

#include <vector>

#include "Declear.h"
#include "IChannelCallBack.h"
#include "Task.h"
#include "Mutex.h"
#include "TimeStamp.h"

struct EventLoop: public IChannelCallBack {
public:
    EventLoop();
    ~EventLoop();
    void loop();
    void update(Channel* channel);
    void queueInLoop(Task& task);
    void runInLoop(Task& task);
    int64_t runAt(TimeStamp when, IRun0* pRun);
    int64_t runAfter(double delay, IRun0* pRun);
    int64_t runEvery(double interval, IRun0* pRun);
    void cancelTimer(int64_t timerfd);
    bool isInLoopThread();

    virtual void handleRead();
    virtual void handleWrite();

private:
    void wakeup();
    int createEventfd();
    void doPendingFuctors();

    bool _quit;
    bool _callingPendingFactors;
    Epoll* _poller;
    int _eventfd;
    const pid_t _threadId;
    Channel* _wakeupChannel;
    MutexLock _mutex;
    std::vector<Task> _pendingFuctors;
    TimerQueue* _pTimerQueue;
};

#endif //CPP_MUDUO_LEARNING_EVENTLOOP_H
