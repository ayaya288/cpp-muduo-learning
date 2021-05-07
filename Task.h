//
// Created by 水瀬あくあ on 2021/5/7.
//

#ifndef CPP_MUDUO_LEARNING_TASK_H
#define CPP_MUDUO_LEARNING_TASK_H

#include "Declear.h"
#include <string>

class Task {
public:
    Task(IRun0* func);
    Task(IRun2* func, const std::string& str, void* param);
    void doTask();
private:
    IRun0* _func0;
    IRun2* _func2;
    std::string _str;
    void* _param;
};

#endif //CPP_MUDUO_LEARNING_TASK_H
