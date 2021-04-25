//
// Created by 水瀬あくあ on 2021/4/24.
//

#ifndef CPP_MUDUO_LEARNING_TCPSERVER_H
#define CPP_MUDUO_LEARNING_TCPSERVER_H

#include <sys/epoll.h>
#include <map>

#include "Define.h"
#include "Declear.h"
#include "IChannelCallBack.h"

class TcpServer: public IChannelCallBack{
public:
    TcpServer();
    ~TcpServer();
    void start();
    virtual void OnIn(int sockfd);
private:
    int createAndListen();

    int _epollfd;
    int _listenfd;
    struct epoll_event _events[MAX_EVENTS];
    std::map<int, Channel*> _channels;
};

#endif //CPP_MUDUO_LEARNING_TCPSERVER_H
