//
// Created by 水瀬あくあ on 2021/4/24.
//

#ifndef CPP_MUDUO_LEARNING_TCPSERVER_H
#define CPP_MUDUO_LEARNING_TCPSERVER_H

#include <sys/epoll.h>
#include <map>

#include "Define.h"
#include "Declear.h"
#include "EventLoop.h"
#include "IAcceptorCallBack.h"

class TcpServer: public IAcceptorCallBack{
public:
    TcpServer(EventLoop* loop);
    ~TcpServer();
    void start();
    virtual void newConnection(int sockfd);
private:
    int _epollfd;
    struct epoll_event _events[MAX_EVENTS];
    std::map<int, TcpConnection*> _connections;
    Acceptor* _pAcceptor;
    EventLoop* _loop;
};

#endif //CPP_MUDUO_LEARNING_TCPSERVER_H
