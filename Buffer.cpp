//
// Created by 水瀬あくあ on 2021/4/30.
//

#include "Buffer.h"

Buffer::Buffer() = default;

Buffer::~Buffer() = default;

const char * Buffer::peek() {
    return _buf.c_str();
}

int Buffer::readableBytes() {
    return static_cast<int>(_buf.size());
}

void Buffer::retrieve(size_t len) {
    _buf = _buf.substr(len, _buf.size());
}

void Buffer::append(const std::string &buf) {
    _buf.append(buf);
}

std::string Buffer::retrieveAllAsString() {
    return retrieveAsString(readableBytes());
}

std::string Buffer::retrieveAsString(size_t len) {
    std::string result(peek(), len);
    retrieve(len);
    return result;
}