//
// Created by 水瀬あくあ on 2021/5/2.
//

#include "TimeStamp.h"
#include <sys/time.h>
#include <cinttypes>

TimeStamp::TimeStamp(double microSeconds):
    _microSecondsSinceEpoch(microSeconds) {}

TimeStamp::~TimeStamp() = default;

bool TimeStamp::valid() {
    return _microSecondsSinceEpoch > 0;
}

int64_t TimeStamp::microSecondsSinceEpoch() {
    return _microSecondsSinceEpoch;
}

std::string TimeStamp::toString() const {
    char buf[32] = {0};
    int64_t seconds = _microSecondsSinceEpoch / kMicroSecondsPerSecond;
    int64_t microseconds = _microSecondsSinceEpoch % kMicroSecondsPerSecond;
    snprintf(buf, sizeof(buf)-1, "%" PRId64 ".%06" PRId64 "", seconds, microseconds);
    return buf;
}

TimeStamp TimeStamp::now() {
    return TimeStamp(TimeStamp::nowMicroSeconds());
}

TimeStamp TimeStamp::nowAfter(double seconds) {
    return TimeStamp(TimeStamp::nowMicroSeconds() + kMicroSecondsPerSecond * seconds);
}

double TimeStamp::nowMicroSeconds() {
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    int64_t seconds = tv.tv_sec;
    return seconds * kMicroSecondsPerSecond + tv.tv_usec;
}

bool operator<(TimeStamp l, TimeStamp r) {
    return l.microSecondsSinceEpoch() < r.microSecondsSinceEpoch();
}

bool operator==(TimeStamp l, TimeStamp r) {
    return l.microSecondsSinceEpoch() == r.microSecondsSinceEpoch();
}