//
// Created by 水瀬あくあ on 2021/4/26.
//

#include "TcpConnection.h"
#include "IAcceptorCallBack.h"
#include "Define.h"
#include "Task.h"

#include <iostream>
#include <unistd.h>
#include <cstring>

TcpConnection::TcpConnection(EventLoop* loop, int sockfd):
    _loop(loop),
    _sockfd(sockfd),
    _pUser(nullptr) {
    _pChannel = new Channel(_loop, sockfd);
    _pChannel->setCallBack(this);
    _pChannel->enableReading();
}

TcpConnection::~TcpConnection() {
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
        std::string linestr(line, read_length);
        _inBuf.append(line);
        _pUser->onMessage(this, &_inBuf);
    }

}

void TcpConnection::handleWrite() {
    int sockfd = _pChannel->getSockfd();
    if(_pChannel->isWriting()) {
        int n = write(sockfd, _outBuf.peek(), _outBuf.readableBytes());
        if(n > 0) {
            _outBuf.retrieve(n);
            if(_outBuf.readableBytes() == 0) {
                _pChannel->disableWriting();
                Task task(this);
                _loop->queueInLoop(task);
            }
        }
    }
}

void TcpConnection::send(const std::string &message) {
    if(_loop->isInLoopThread()) {
        sendInLoop(message);
    } else {
        Task task(this, message, this);
        _loop->runInLoop(task);
    }
}

void TcpConnection::sendInLoop(const std::string &message) {
    int n = 0;
    if(_outBuf.readableBytes() == 0) {
        n = write(_sockfd, message.c_str(), message.size());
        if(n < 0) {
            std::cout << "write error!" << std::endl;
        }
        if(n == static_cast<int>(message.size())) {
            Task task(this);
            _loop->runInLoop(task);
        }
    }
    if(n < static_cast<int>(message.size())) {
        _outBuf.append(message);
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

void TcpConnection::run0() {
    _pUser->onWriteComplete(this);
}

void TcpConnection::run2(const std::string &message, void *param) {
    sendInLoop(message);
}