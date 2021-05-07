//
// Created by 水瀬あくあ on 2021/5/2.
//

#ifndef CPP_MUDUO_LEARNING_TIMERQUEUE_H
#define CPP_MUDUO_LEARNING_TIMERQUEUE_H

#include "Declear.h"
#include "IChannelCallBack.h"
#include "IRun.h"
#include "TimeStamp.h"
#include "Timer.h"

#include <vector>
#include <set>

class TimerQueue: public IChannelCallBack
                ,public IRun2 {
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    void doAddTimer(Timer* timer);
    void doCancelTimer(Timer* timer);
    int64_t addTimer(IRun0* pRun, const TimeStamp& when, double interval);
    void cancelTimer(int64_t timerId);

    virtual void run2(const std::string& str, void* timer);

    virtual void handleRead();
    virtual void handleWrite();

private:
    typedef std::pair<TimeStamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;

    int createTimerfd();
    std::vector<TimerQueue::Entry> getExpired(TimeStamp now);
    void readTimerfd(int timerfd, TimeStamp now);
    void reset(const std::vector<Entry>& expired, TimeStamp now);
    void resetTimerfd(int timerfd, TimeStamp stamp);
    bool insert(Timer* pItem);
    struct timespec howMuchTimeFromNow(TimeStamp when);

    int _timerfd;
    TimerList _timers;
    EventLoop* _loop;
    Channel* _timerfdChannel;
};

#endif //CPP_MUDUO_LEARNING_TIMERQUEUE_H
