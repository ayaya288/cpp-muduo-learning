//
// Created by 水瀬あくあ on 2021/4/28.
//

#include "EchoServer.h"

#define MESSAGE_LENGTH 9

EchoServer::EchoServer(EventLoop* pLoop):
    _loop(pLoop) {
    _pServer = new TcpServer(_loop);
    _pServer->setCallBack(this);
}

EchoServer::~EchoServer() {
    delete _pServer;
}

void EchoServer::start() {
    _pServer->start();
}

void EchoServer::onConnection(TcpConnection *pConn) {
    //std::cout << "onConnection test successful" << std::endl;
}

void EchoServer::onMessage(TcpConnection *pConn, std::string *data) {
    //临时测试，一次发送长度为MESSAGE_LENGTH的字符串
    while(! data->empty()) {
        std::string message;
        if(data->size() > MESSAGE_LENGTH) {
            message = data->substr(0, MESSAGE_LENGTH);
            *data = data->substr(MESSAGE_LENGTH, data->size());
        } else {
            message = data->substr(0, data->size());
            *data = "";
        }
        pConn->send(message + "\n");
    }
}