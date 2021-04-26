//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H
#define CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H

class IAcceptorCallBack {
public:
    virtual void newConnection(int sockfd) {};
};

#endif //CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H
