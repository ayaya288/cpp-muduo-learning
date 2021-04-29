//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_ACCEPTOR_H
#define CPP_MUDUO_LEARNING_ACCEPTOR_H

#include "IChannelCallBack.h"
#include "IAcceptorCallBack.h"
#include "Channel.h"
#include "EventLoop.h"

class Acceptor: public IChannelCallBack{
public:
    Acceptor(EventLoop* loop);
    ~Acceptor();

    virtual void handleRead();
    virtual void handleWrite();
    void setCallBack(IAcceptorCallBack* pCallBack);
    void start();
private:
    int createAndListen();
    EventLoop* _loop;
    int _listenfd;
    Channel* _pAcceptChannel;
    IAcceptorCallBack* _pCallBack;
};

#endif //CPP_MUDUO_LEARNING_ACCEPTOR_H
