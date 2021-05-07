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
        Task task(this);
        Thread* p = new Thread(task);
        _threads.push_back(p);
        p->start();
    }
}

void ThreadPool::addTask(Task& task) {
    _tasks.put(task);
}

void ThreadPool::run0() {
    runInThread();
}

[[noreturn]] void ThreadPool::runInThread() {
    while(true) {
        _tasks.take().doTask();
    }
}