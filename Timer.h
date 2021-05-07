//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_TIMER_H
#define CPP_MUDUO_LEARNING_TIMER_H
#include "Declear.h"
#include "IRun.h"

class Timer{
public:
    Timer(TimeStamp stamp, IRun0* pRun, double interval):
            _stamp(stamp),
            _id(stamp),
            _pRun0(pRun),
            _interval(interval) {};
    TimeStamp getStamp() {
        return _stamp;
    }
    TimeStamp getID() {
        return _id;
    }
    void timeout() {
        _pRun0->run0();
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
    IRun0* _pRun0;
    double _interval;
};
#endif //CPP_MUDUO_LEARNING_TIMER_H
