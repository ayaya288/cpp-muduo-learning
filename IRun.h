//
// Created by 水瀬あくあ on 2021/4/30.
//

#ifndef CPP_MUDUO_LEARNING_IRUN_H
#define CPP_MUDUO_LEARNING_IRUN_H
#include <string>

class IRun0 {
public:
    virtual void run0() = 0;
};

class IRun2 {
public:
    virtual void run2(const std::string& str, void* param) = 0;
};

#endif //CPP_MUDUO_LEARNING_IRUN_H
