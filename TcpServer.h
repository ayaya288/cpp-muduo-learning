//
// Created by 水瀬あくあ on 2021/4/24.
//

#ifndef CPP_MUDUO_LEARNING_TCPSERVER_H
#define CPP_MUDUO_LEARNING_TCPSERVER_H

#include <sys/epoll.h>
#include <map>

#include "IRun.h"
#include "Define.h"
#include "Declear.h"
#include "EventLoop.h"
#include "EchoServer.h"
#include "IAcceptorCallBack.h"

class TcpServer: public IAcceptorCallBack {
public:
    TcpServer(EventLoop* loop);
    ~TcpServer();
    void start();
    void setCallBack(IMuduoUser* user);
    virtual void newConnection(int sockfd);
    virtual void closedConnection(int sockfd);

private:
    struct epoll_event _events[MAX_EVENTS];
    std::map<int, TcpConnection*> _connections;
    Acceptor* _pAcceptor;
    EventLoop* _loop;
    IMuduoUser* _pUser;
};

#endif //CPP_MUDUO_LEARNING_TCPSERVER_H
