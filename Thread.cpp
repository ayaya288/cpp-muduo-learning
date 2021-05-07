//
// Created by 水瀬あくあ on 2021/5/7.
//

#include "Thread.h"
#include "BlockingQueue.h"

#include <unistd.h>
#include <sys/syscall.h>

void* globalRun(void* arg) {
    ((Thread*)arg) -> run();
    return nullptr;
}

Thread::Thread(IRun *pRun)
    :_run(pRun) {

}

Thread::~Thread() = default;

void Thread::start() {
    pthread_t t;
    pthread_create(&t, nullptr, globalRun, this);
}

void Thread::run() {
    _run->run(nullptr);
}

pid_t Thread::gettid() {
    return static_cast<pid_t>(syscall(SYS_gettid));
}