//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_TCPCONNECTION_H
#define CPP_MUDUO_LEARNING_TCPCONNECTION_H

#include "IChannelCallBack.h"
#include "Channel.h"

class TcpConnection: public IChannelCallBack{
public:
    TcpConnection(int epollfd, int sockfd);
    ~TcpConnection();
    virtual void OnIn(int sockfd);

private:
    int _epollfd;
    int _sockfd;
    Channel* _pChannel;
};

#endif //CPP_MUDUO_LEARNING_TCPCONNECTION_H
