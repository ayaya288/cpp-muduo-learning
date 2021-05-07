//
// Created by 水瀬あくあ on 2021/4/28.
//

#include "EchoServer.h"
#include "Task.h"
#include <sys/syscall.h>
#include <iostream>
#include <unistd.h>

#define MESSAGE_LENGTH 9

EchoServer::EchoServer(EventLoop* pLoop):
    _loop(pLoop),
    _pServer(new TcpServer(pLoop)),
    _timer(-1),
    _index(0) {
    _pServer->setCallBack(this);
}

EchoServer::~EchoServer() {
    delete _pServer;
}

void EchoServer::start() {
    _pServer->start();
    _threadPool.start(3);
}

void EchoServer::onConnection(TcpConnection *pConn) {
    std::cout << "onConnection test successful" << std::endl;
}

void EchoServer::onMessage(TcpConnection *pConn, Buffer* pBuf) {
    //临时测试，一次发送长度为MESSAGE_LENGTH的字符串
    while(pBuf->readableBytes() != 0) {
        std::string message;
        if(pBuf->readableBytes() > MESSAGE_LENGTH) {
            message = pBuf->retrieveAsString(MESSAGE_LENGTH);
        } else {
            message = pBuf->retrieveAllAsString();
        }
        Task task(this, message, pConn);
        _threadPool.addTask(task);
        _index = 3;
        _loop->runAfter(3, this);
    }
}

void EchoServer::onWriteComplete(TcpConnection *pConn) {
    std::cout << "onWriteComplete test successful" << std::endl;
}

void EchoServer::run0() {
    if(_index) {
        std::cout << _index << std::endl;
        --_index;
    }
}

void EchoServer::run2(const std::string& str, void* tcp) {
    std::cout << "fib(30) = " << fib(30) << "tid = " << static_cast<pid_t>(syscall(SYS_gettid)) << std::endl;
    ((TcpConnection*)tcp)->send(str + "\n");
}

int EchoServer::fib(int n) {
    if(n <= 0) return 0;
    int f0 = 0;
    int f1 = 1;
    n -= 2;
    while(n > 0) {
        int buf = f0;
        f0 = f1;
        f1 = f0 + buf;
        --n;
    }
    return f0 + f1;
}