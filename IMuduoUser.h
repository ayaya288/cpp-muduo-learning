//
// Created by 水瀬あくあ on 2021/4/28.
//

#ifndef CPP_MUDUO_LEARNING_IMUDUOUSER_H
#define CPP_MUDUO_LEARNING_IMUDUOUSER_H

#include "Declear.h"
#include "TcpConnection.h"
#include <string>

class IMuduoUser {
public:
    virtual void onConnection(TcpConnection* pConn) = 0;
    virtual void onMessage(TcpConnection* pConn, std::string* data) = 0;
};

#endif //CPP_MUDUO_LEARNING_IMUDUOUSER_H
