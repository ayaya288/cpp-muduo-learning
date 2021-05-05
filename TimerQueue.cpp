//
// Created by 水瀬あくあ on 2021/5/2.
//

#include <sys/timerfd.h>
#include <cinttypes>
#include <iostream>
#include <cstring>
#include <unistd.h>

#include "TimerQueue.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TimeStamp.h"

TimerQueue::TimerQueue(EventLoop *loop):
    _timerfd(createTimerfd()),
    _loop(loop),
    _timerfdChannel(new Channel(_loop, _timerfd)),
    _addTimerWrapper(new AddTimerWrapper(this)),
    _cancelTimerWrapper(new CancelTimerWrapper(this)) {
    _timerfdChannel->setCallBack(this);
    _timerfdChannel->enableReading();
}

TimerQueue::~TimerQueue() {
    close(_timerfd);
    delete _timerfdChannel;
    delete _addTimerWrapper;
    delete _cancelTimerWrapper;
}

void TimerQueue::doAddTimer(void *param) {
    Timer* pTimer = static_cast<Timer*>(param);
    bool earliestChanged = insert(pTimer);
    if(earliestChanged) {
        resetTimerfd(_timerfd, pTimer->getStamp());
    }
}

void TimerQueue::doCancelTimer(void *param) {
    auto* pTimer = static_cast<Timer*>(param);
    Entry e(pTimer->getID(), pTimer);
    for(auto& it : _timers) {
        if(it.second == pTimer) {
            _timers.erase(it);
            break;
        }
    }
}

/// Add a timer to the system
/// @param pRun: callback interface
/// @param when: time
/// @param interval: 0-happen only once, n-happen every n seconds
/// @return the process unique id of the timer
int64_t TimerQueue::addTimer(IRun *pRun, const TimeStamp& when, double interval) {
    Timer* pTimer = new Timer(when, pRun, interval);
    _loop->queueLoop(_addTimerWrapper, pTimer);
    return reinterpret_cast<int64_t>(pTimer);
}

void TimerQueue::cancelTimer(int64_t timerId) {
    _loop->queueLoop(_cancelTimerWrapper, (void*)timerId);
}

void TimerQueue::handleRead() {
    TimeStamp now(TimeStamp::now());
    readTimerfd(_timerfd, now);

    std::vector<Entry> expired = getExpired(now);
    for(auto& it : expired) {
        it.second->run();
    }
    reset(expired, now);
}

void TimerQueue::handleWrite() {

}

int TimerQueue::createTimerfd() {
    int timefd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK|TFD_CLOEXEC);
    if(timefd < 0) {
        std::cout << "faild in timefd create" << std::endl;
    }
    return timefd;
}

std::vector<TimerQueue::Entry> TimerQueue::getExpired(TimeStamp now) {
    std::vector<Entry> expired;
    Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
    auto end = _timers.lower_bound(sentry);
    copy(_timers.begin(), end, back_inserter(expired));
    _timers.erase(_timers.begin(), end);
    return expired;
}

void TimerQueue::readTimerfd(int timerfd, TimeStamp now) {
    uint64_t howmany;
    ssize_t n = read(timerfd, &howmany, sizeof(howmany));
    if(n != sizeof(howmany)) {
        std::cout << "Timer::readTimerfd() error" << std::endl;
    }
}

void TimerQueue::reset(const std::vector<Entry> &expired, TimeStamp now) {
    for(auto it : expired) {
        if(it.second->isRepeat()) {
            it.second->moveToNext();
            insert(it.second);
        }
    }
    TimeStamp nextExpire;
    if(!_timers.empty()) {
        nextExpire = _timers.begin()->second->getStamp();
    }
    if(nextExpire.valid()) {
        resetTimerfd(_timerfd, nextExpire);
    }
}

void TimerQueue::resetTimerfd(int timerfd, TimeStamp stamp) {
    struct itimerspec newValue;
    struct itimerspec oldValue;
    bzero(&newValue, sizeof(newValue));
    bzero(&oldValue, sizeof(oldValue));
    newValue.it_value = howMuchTimeFromNow(stamp);
    int ret = timerfd_settime(timerfd, 0, &newValue, &oldValue);
    if(ret) {
        std::cout << "timefd_settime error" << std::endl;
    }
}

bool TimerQueue::insert(Timer *pItem) {
    bool earliestChanged = false;
    TimeStamp when = pItem->getStamp();
    auto it = _timers.begin();
    if(it == _timers.end() || when < it->first) {
        earliestChanged = true;
    }
    std::pair<TimerList::iterator, bool> result = _timers.insert(Entry(when, pItem));
    if(! result.second) {
        std::cout << "_timers,insert() error" << std::endl;
    }
    return earliestChanged;
}

struct timespec TimerQueue::howMuchTimeFromNow(TimeStamp when) {
    int64_t microseconds = when.microSecondsSinceEpoch() - TimeStamp::now().microSecondsSinceEpoch();
    if(microseconds < 100) {
        microseconds = 100;
    }
    struct timespec ts;
    ts.tv_sec = static_cast<time_t>(microseconds / TimeStamp::kMicroSecondsPerSecond);
    ts.tv_nsec = static_cast<long>((microseconds % TimeStamp::kMicroSecondsPerSecond) * 1000);
    return ts;
}