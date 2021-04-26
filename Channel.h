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
    int getEvents();
    int getSockfd();
    void enableReading();
private:
    void update();
    EventLoop* _loop;
    int _sockfd;
    int _events;
    int _revents;
    IChannelCallBack* _callBack;
};

#endif //CPP_MUDUO_LEARNING_CHANNEL_H
