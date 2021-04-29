//
// Created by 水瀬あくあ on 2021/4/25.
//

#ifndef CPP_MUDUO_LEARNING_CHANNEL_H
#define CPP_MUDUO_LEARNING_CHANNEL_H

#include "Declear.h"
#include "EventLoop.h"

class Channel {
public:
    Channel(EventLoop* loop, int sockfd);
    ~Channel();
    void setCallBack(IChannelCallBack* callBack);
    void handleEvent();
    void setRevents(int revents);
    void enableReading();
    void enableWriting();
    void disableWriting();
    bool isWriting();

    int getEvents();
    int getSockfd();
private:
    void update();
    int _sockfd;
    int _events;
    int _revents;
    IChannelCallBack* _pCallBack;
    EventLoop* _loop;

};

#endif //CPP_MUDUO_LEARNING_CHANNEL_H
