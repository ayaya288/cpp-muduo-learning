//
// Created by 水瀬あくあ on 2021/4/28.
//

#ifndef CPP_MUDUO_LEARNING_ECHOSERVER_H
#define CPP_MUDUO_LEARNING_ECHOSERVER_H
#include "IMuduoUser.h"
#include "TcpServer.h"
#include "IRun.h"

class EchoServer: public IMuduoUser,
                  public IRun {
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    virtual void onConnection(TcpConnection* pConn);
    virtual void onMessage(TcpConnection* pConn, Buffer* pBuf);
    virtual void onWriteComplete(TcpConnection* pConn);

    virtual void run(void* param);
private:
    EventLoop* _loop;
    TcpServer* _pServer;
    int _index;
    int64_t _timer;
};

#endif //CPP_MUDUO_LEARNING_ECHOSERVER_H
