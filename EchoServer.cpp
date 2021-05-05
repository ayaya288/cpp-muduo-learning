//
// Created by 水瀬あくあ on 2021/4/28.
//

#include "EchoServer.h"
#include <iostream>

#define MESSAGE_LENGTH 9

EchoServer::EchoServer(EventLoop* pLoop):
    _loop(pLoop),
    _pServer(nullptr),
    _timer(-1),
    _index(0) {
    _pServer = new TcpServer(_loop);
    _pServer->setCallBack(this);
}

EchoServer::~EchoServer() = default;

void EchoServer::start() {
    _pServer->start();
}

void EchoServer::onConnection(TcpConnection *pConn) {
    std::cout << "onConnection test successful" << std::endl;
}

void EchoServer::onMessage(TcpConnection *pConn, Buffer* pBuf) {
    //临时测试，一次发送长度为MESSAGE_LENGTH的字符串
    while(pBuf->readableBytes() != 0) {
        std::string message;
        if(pBuf->readableBytes() > MESSAGE_LENGTH) {
            message = pBuf->retrieveAsString(MESSAGE_LENGTH);
        } else {
            message = pBuf->retrieveAllAsString();
        }
        pConn->send(message + "\n");
    }
    _timer = _loop->runEvery(3, this);
}

void EchoServer::onWriteComplete(TcpConnection *pConn) {
    std::cout << "onWriteComplete test successful" << std::endl;
}

void EchoServer::run(void *param) {
    std::cout << _index << std::endl;
    if(_index++ == 3) {
        _loop->cancelTimer(_timer);
        _index = 0;
    }
}