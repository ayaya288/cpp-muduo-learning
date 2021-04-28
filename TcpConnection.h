//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_TCPCONNECTION_H
#define CPP_MUDUO_LEARNING_TCPCONNECTION_H

#include "IChannelCallBack.h"
#include "IMuduoUser.h"
#include "Channel.h"
#include "EventLoop.h"
#include <string>

class TcpConnection: public IChannelCallBack{
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();
    virtual void OnIn(int sockfd);

    void send(const std::string& message);
    void connectEstablished();
    void setUser(IMuduoUser* pUser);
    void setCallBack(IAcceptorCallBack* pCallBack);

private:
    EventLoop* _loop;
    int _sockfd;
    Channel* _pChannel;
    IMuduoUser* _pUser;
    IAcceptorCallBack* _pCallBack;
};

#endif //CPP_MUDUO_LEARNING_TCPCONNECTION_H
