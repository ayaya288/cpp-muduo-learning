//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_THREAD_H
#define CPP_MUDUO_LEARNING_THREAD_H

#include "BlockingQueue.h"
#include "IRun.h"

#include <pthread.h>

class Thread {
public:
    explicit Thread(IRun* pRun);
    ~Thread();
    void start();
    void run();
    pid_t gettid();

private:
    IRun* _run;
};
#endif //CPP_MUDUO_LEARNING_THREAD_H
