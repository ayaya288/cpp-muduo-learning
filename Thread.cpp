//
// Created by 水瀬あくあ on 2021/5/7.
//

#include "Thread.h"
#include "BlockingQueue.h"

#include <unistd.h>
#include <sys/syscall.h>

void* globalRun(void* arg) {
    ((Task*)arg) -> doTask();
    return nullptr;
}

Thread::Thread(Task& task)
    : _task(task) {

}

Thread::~Thread() = default;

void Thread::start() {
    pthread_t t;
    pthread_create(&t, nullptr, globalRun, this);
}

pid_t Thread::gettid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}