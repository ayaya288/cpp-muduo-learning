//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_THREAD_H
#define CPP_MUDUO_LEARNING_THREAD_H

#include "BlockingQueue.h"
#include "IRun.h"
#include "Task.h"

#include <pthread.h>

class Thread {
public:
    explicit Thread(Task& task);
    ~Thread();
    void start();
    pid_t gettid();

private:
    Task _task;
};
#endif //CPP_MUDUO_LEARNING_THREAD_H
