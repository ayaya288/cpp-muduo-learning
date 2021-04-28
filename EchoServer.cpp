//
// Created by 水瀬あくあ on 2021/4/28.
//

#include "EchoServer.h"

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

void EchoServer::onMessage(TcpConnection *pConn, std::string &data) {
    //std::cout << "onMessage test successful" << std::endl;
    pConn->send(data);
}