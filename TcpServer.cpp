//
// Created by 水瀬あくあ on 2021/4/24.
//

#include <iostream>
#include <vector>

#include "TcpServer.h"
#include "Channel.h"
#include "Acceptor.h"
#include "TcpConnection.h"

TcpServer::TcpServer(EventLoop* loop):
    _pAcceptor(nullptr),
    _loop(loop),
    _pUser(nullptr) {}

TcpServer::~TcpServer() {
    delete _pAcceptor;
}

void TcpServer::newConnection(int sockfd) {
    TcpConnection* tcp = new TcpConnection(_loop, sockfd); //TODO~:Memory Leak!
    _connections[sockfd] = tcp;
    tcp->setUser(_pUser);
    tcp->connectEstablished();
}

void TcpServer::closedConnection(int sockfd) {
    //未处理
}

void TcpServer::start() {
    //创建Acceptor用于创建listenfd和建立连接
    _pAcceptor = new Acceptor(_loop);
    _pAcceptor->setCallBack(this);
    _pAcceptor->start();
}

void TcpServer::setCallBack(IMuduoUser *user) {
    _pUser = user;
}

