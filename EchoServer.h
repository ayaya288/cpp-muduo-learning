//
// Created by 水瀬あくあ on 2021/4/28.
//

#ifndef CPP_MUDUO_LEARNING_ECHOSERVER_H
#define CPP_MUDUO_LEARNING_ECHOSERVER_H

#include "Define.h"
#include "IMuduoUser.h"
#include "TcpServer.h"
#include "IRun.h"
#include "ThreadPool.h"

class EchoServer: public IMuduoUser,
                  public IRun0,
                  public IRun2 {
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    virtual void onConnection(TcpConnection* pConn);
    virtual void onMessage(TcpConnection* pConn, Buffer* pBuf);
    virtual void onWriteComplete(TcpConnection* pConn);
    virtual void run0();
    virtual void run2(const std::string& str, void* tcp);
private:
    int fib(int n);
    EventLoop* _loop;
    TcpServer* _pServer;
    ThreadPool _threadPool;
    int _index;
    int64_t _timer;
};

#endif //CPP_MUDUO_LEARNING_ECHOSERVER_H
