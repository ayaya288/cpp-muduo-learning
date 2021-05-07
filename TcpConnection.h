//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_TCPCONNECTION_H
#define CPP_MUDUO_LEARNING_TCPCONNECTION_H

#include "IChannelCallBack.h"
#include "IMuduoUser.h"
#include "IRun.h"
#include "Channel.h"
#include "EventLoop.h"
#include "Buffer.h"
#include <string>

class TcpConnection: public IChannelCallBack,
                     public IRun0,
                     public IRun2 {
public:
    TcpConnection(EventLoop* loop, int sockfd);
    ~TcpConnection();
    void send(const std::string& message);
    void sendInLoop(const std::string& message);
    void connectEstablished();
    void setUser(IMuduoUser* pUser);
    void setCallBack(IAcceptorCallBack* pCallBack);
    virtual void handleRead();
    virtual void handleWrite();
    virtual void run0();
    virtual void run2(const std::string& message, void* param);
private:
    EventLoop* _loop;
    int _sockfd;
    Channel* _pChannel;
    IMuduoUser* _pUser;
    IAcceptorCallBack* _pCallBack;
    Buffer _inBuf;
    Buffer _outBuf;
};

#endif //CPP_MUDUO_LEARNING_TCPCONNECTION_H
