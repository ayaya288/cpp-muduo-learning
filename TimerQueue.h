//
// Created by 水瀬あくあ on 2021/5/2.
//

#ifndef CPP_MUDUO_LEARNING_TIMERQUEUE_H
#define CPP_MUDUO_LEARNING_TIMERQUEUE_H

#include "Declear.h"
#include "IChannelCallBack.h"
#include "IRun.h"
#include "TimeStamp.h"

#include <vector>
#include <set>

class TimerQueue: public IChannelCallBack{
public:
    class Timer{
    public:
        Timer(TimeStamp stamp, IRun* pRun, double interval):
            _stamp(stamp),
            _id(stamp),
            _pRun(pRun),
            _interval(interval) {};
        TimeStamp getStamp() {
            return _stamp;
        }
        TimeStamp getID() {
            return _id;
        }
        void run() {
            _pRun->run(this);
        }
        bool isRepeat() {
            return _interval > 0.0;
        }
        void moveToNext() {
            _stamp = TimeStamp::nowAfter(_interval);
        }

    private:
        TimeStamp _stamp;
        TimeStamp _id;
        IRun* _pRun;
        double _interval;
    };

    class AddTimerWrapper: public IRun{
    public:
        AddTimerWrapper(TimerQueue* pQueue):
            _pQueue(pQueue) {};
        virtual void run(void* param) {
            _pQueue->doAddTimer(param);
        }
    private:
        TimerQueue* _pQueue;

    };

    class CancelTimerWrapper: public IRun{
    public:
        CancelTimerWrapper(TimerQueue* pQueue):
            _pQueue(pQueue) {};
        virtual void run(void* param) {
            _pQueue->doCancelTimer(param);
        }
    private:
        TimerQueue* _pQueue;
    };

    TimerQueue(EventLoop* loop);
    ~TimerQueue();
    void doAddTimer(void* param);
    void doCancelTimer(void* param);
    int64_t addTimer(IRun* pRun, const TimeStamp& when, double interval);
    void cancelTimer(int64_t timerId);

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
    AddTimerWrapper* _addTimerWrapper;
    CancelTimerWrapper* _cancelTimerWrapper;
};

#endif //CPP_MUDUO_LEARNING_TIMERQUEUE_H
