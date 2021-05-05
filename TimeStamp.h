//
// Created by 水瀬あくあ on 2021/5/2.
//

#ifndef CPP_MUDUO_LEARNING_TIMESTAMP_H
#define CPP_MUDUO_LEARNING_TIMESTAMP_H

#include <sys/types.h>
#include <string>

class TimeStamp{
public:
    TimeStamp(double microSeconds = 0.0);
    ~TimeStamp();
    bool valid();
    int64_t microSecondsSinceEpoch();
    std::string toString() const;

    static TimeStamp now();
    static TimeStamp nowAfter(double seconds);
    static double nowMicroSeconds();
    static const int kMicroSecondsPerSecond = 1000 * 1000;
private:
    int64_t _microSecondsSinceEpoch;
};

bool operator<(TimeStamp l, TimeStamp r);
bool operator==(TimeStamp l, TimeStamp r);

#endif //CPP_MUDUO_LEARNING_TIMESTAMP_H
