//
// Created by 水瀬あくあ on 2021/4/26.
//

#include "TcpConnection.h"
#include "Define.h"

#include <iostream>
#include <unistd.h>
#include <cstring>

TcpConnection::TcpConnection(EventLoop* loop, int sockfd):
    _loop(loop),
    _sockfd(sockfd),
    _pUser(nullptr),
    _inBuf(new std::string()),
    _outBuf(new std::string()) {
    _pChannel = new Channel(_loop, sockfd);
    _pChannel->setCallBack(this);
    _pChannel->enableReading();
}

TcpConnection::~TcpConnection() {
    delete _inBuf;
    delete _outBuf;
    delete _pChannel;
}

void TcpConnection::handleRead() {
    int sockfd = _pChannel->getSockfd();
    if(sockfd < 0) {
        std::cout << "socket id minus error, errno: "<< errno << std::endl;
        return;
    }
    // 开始处理输入的数据
    char line[MAX_LINE];
    bzero(line, MAX_LINE);
    int read_length = read(sockfd, line, MAX_LINE);
    if(read_length < 0) {
        if(errno == ECONNRESET) {
            std::cout << "ECONNRESET error, closed: " << sockfd << std::endl;
            close(sockfd);
        }
    } else if(read_length == 0) {
        std::cout << "Client Connection closed: " << sockfd << std::endl;
        close(sockfd);
    } else {
        _inBuf->append(line, read_length);
        _pUser->onMessage(this, _inBuf);
    }

}

void TcpConnection::handleWrite() {
    int sockfd = _pChannel->getSockfd();
    if(_pChannel->isWriting()) {
        int n = write(sockfd, _outBuf->c_str(), _outBuf->size());
        if(n > 0) {
            *_outBuf = _outBuf->substr(n, _outBuf->size());
            if(_outBuf->empty()) {
                _pChannel->disableWriting();
            }
        }
    }
}

void TcpConnection::send(const std::string &message) {
    int n = 0;
    if(_outBuf->empty()) {
        n = write(_sockfd, message.c_str(), message.size());
        if(n < 0) {
            std::cout << "write error!" << std::endl;
        }
    }
    if(n < static_cast<int>(message.size())) {
        *_outBuf += message.substr(n, message.size());
        if(_pChannel->isWriting()) {
            _pChannel->enableWriting();
        }
    }
}

void TcpConnection::connectEstablished() {
    if(_pUser) {
        _pUser->onConnection(this);
    }
}

void TcpConnection::setUser(IMuduoUser *pUser) {
    _pUser = pUser;
}

void TcpConnection::setCallBack(IAcceptorCallBack *pCallBack) {
    _pCallBack = pCallBack;
}