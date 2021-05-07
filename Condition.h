//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_CONDITION_H
#define CPP_MUDUO_LEARNING_CONDITION_H

#include "pthread.h"
#include "Mutex.h"

class Condition {
public:
    Condition(MutexLock& mutex)
        :_mutex(mutex) {
        pthread_cond_init(&_condId, nullptr);
    }
    ~Condition() {
        pthread_cond_destroy(&_condId);
    }
    void wait() {
        pthread_cond_wait(&_condId, _mutex.getPthreadMutex());
    }
    void notify() {
        pthread_cond_signal(&_condId);
    }
    void notifiAll() {
        pthread_cond_broadcast(&_condId);
    }

private:
    MutexLock& _mutex;
    pthread_cond_t _condId;
};

#endif //CPP_MUDUO_LEARNING_CONDITION_H
