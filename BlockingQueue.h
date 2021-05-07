//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_BLOCKINGQUEUE_H
#define CPP_MUDUO_LEARNING_BLOCKINGQUEUE_H

#include <deque>
#include "Condition.h"
#include "Mutex.h"

template<class T>
class BlockingQueue {
public:
    BlockingQueue()
        : _cond(_mutex) {

    }
    ~BlockingQueue() = default;
    void put(const T& one) {
        MutexLockGuard lock(_mutex);
        _queue.push_back(one);
        _cond.notify();
    }
    T take() {
        MutexLockGuard lock(_mutex);
        while(_queue.empty()) {
            _cond.wait();
        }
        T front(_queue.front());
        _queue.pop_front();
        return front;
    }

private:
    std::deque<T> _queue;
    MutexLock _mutex;
    Condition _cond;
};

#endif //CPP_MUDUO_LEARNING_BLOCKINGQUEUE_H
