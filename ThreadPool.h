//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_THREADPOOL_H
#define CPP_MUDUO_LEARNING_THREADPOOL_H

#include "IRun.h"
#include "Declear.h"
#include "BlockingQueue.h"

#include <vector>

class ThreadPool: public IRun {
public:
    ThreadPool();
    ~ThreadPool();
    void start(int numThreads);
    void addTask(IRun* ptask);
    virtual void run(void* param);

private:
    [[noreturn]] void runInThread();
    BlockingQueue<IRun*> _tasks;
    std::vector<Thread*> _threads;
};

#endif //CPP_MUDUO_LEARNING_THREADPOOL_H
