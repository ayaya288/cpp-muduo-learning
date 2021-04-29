//
// Created by 水瀬あくあ on 2021/4/25.
//

#ifndef CPP_MUDUO_LEARNING_ICHANNELCALLBACK_H
#define CPP_MUDUO_LEARNING_ICHANNELCALLBACK_H

class IChannelCallBack {
public:
    virtual void handleRead() = 0;
    virtual void handleWrite() = 0;
};

#endif //CPP_MUDUO_LEARNING_ICHANNELCALLBACK_H
