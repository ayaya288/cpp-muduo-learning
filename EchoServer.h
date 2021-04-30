//
// Created by 水瀬あくあ on 2021/4/28.
//

#ifndef CPP_MUDUO_LEARNING_ECHOSERVER_H
#define CPP_MUDUO_LEARNING_ECHOSERVER_H
#include "IMuduoUser.h"
#include "TcpServer.h"

class EchoServer: public IMuduoUser {
public:
    EchoServer(EventLoop* pLoop);
    ~EchoServer();
    void start();
    virtual void onConnection(TcpConnection* pConn);
    virtual void onMessage(TcpConnection* pConn, Buffer* pBuf);
    virtual void onWriteComplete(TcpConnection* pConn);
private:
    EventLoop* _loop;
    TcpServer* _pServer;
};

#endif //CPP_MUDUO_LEARNING_ECHOSERVER_H
