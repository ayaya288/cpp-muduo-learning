//
// Created by 水瀬あくあ on 2021/4/30.
//

#ifndef CPP_MUDUO_LEARNING_BUFFER_H
#define CPP_MUDUO_LEARNING_BUFFER_H

#include "Declear.h"
#include <string>

class Buffer{
public:
    Buffer();
    ~Buffer();
    const char* peek();
    int readableBytes();
    void retrieve(size_t len);
    void append(const std::string& buf);
    std::string retrieveAllAsString();
    std::string retrieveAsString(size_t len);
private:
    std::string _buf;
};

#endif //CPP_MUDUO_LEARNING_BUFFER_H
