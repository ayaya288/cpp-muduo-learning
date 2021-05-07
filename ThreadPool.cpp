//
// Created by 水瀬あくあ on 2021/5/7.
//

#include "ThreadPool.h"
#include "Thread.h"

#include <pthread.h>
#include <iostream>
#include <sstream>

ThreadPool::ThreadPool() = default;

ThreadPool::~ThreadPool() = default;

void ThreadPool::start(int numThreads) {
    _threads.reserve(numThreads);
    for(int i = 0; i < numThreads; i++) {
        Thread* p = new Thread(this);
        _threads.push_back(p);
        p->start();
    }
}

void ThreadPool::addTask(IRun *ptask) {
    _tasks.put(ptask);
}

void ThreadPool::run(void *param) {
    runInThread();
}

[[noreturn]] void ThreadPool::runInThread() {
    while(true) {
        IRun* task =reinterpret_cast<IRun*>(_tasks.take());
        task->run(nullptr);
    }
}