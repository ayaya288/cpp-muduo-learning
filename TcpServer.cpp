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

TcpServer::~TcpServer() = default;

void TcpServer::newConnection(int sockfd) {
    //TODO:内存泄漏
    TcpConnection* tcp = new TcpConnection(_loop, sockfd);
    _connections[sockfd] = tcp;
    tcp->setUser(_pUser);
    tcp->connectEstablished();
}

void TcpServer::start() {
    //创建Acceptor用于创建listenfd和建立连接
    //TODO:内存泄漏
    _pAcceptor = new Acceptor(_loop);
    _pAcceptor->setCallBack(this);
    _pAcceptor->start();
}

void TcpServer::setCallBack(IMuduoUser *user) {
    _pUser = user;
}

