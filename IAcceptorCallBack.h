//
// Created by 水瀬あくあ on 2021/4/26.
//

#ifndef CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H
#define CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H
#include "Declear.h"

class IAcceptorCallBack {
public:
    virtual void newConnection(int sockfd) = 0;
};

#endif //CPP_MUDUO_LEARNING_IACCEPTORCALLBACK_H
