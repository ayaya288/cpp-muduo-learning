//
// Created by 水瀬あくあ on 2021/4/24.
//

#include <iostream>
#include <cerrno>
#include <vector>

#include "TcpServer.h"
#include "Channel.h"
#include "Acceptor.h"
#include "TcpConnection.h"

TcpServer::TcpServer():
    _epollfd(-1),
    _pAcceptor(nullptr) {}

TcpServer::~TcpServer() = default;

void TcpServer::newConnection(int sockfd) {
    //TODO:内存泄漏
    TcpConnection* tcp = new TcpConnection(_epollfd, sockfd);
    _connections[sockfd] = tcp;
}

void TcpServer::start() {
    //创建epoll
    _epollfd = epoll_create(1);
    if(_epollfd <= 0) {
        std::cout << "epoll error, error:" << _epollfd << std::endl;
        exit(EPOLL_ERROR);
    }
    //创建Acceptor用于创建listenfd和建立连接
    //TODO:内存泄漏
    _pAcceptor = new Acceptor(_epollfd);
    _pAcceptor->setCallBack(this);
    _pAcceptor->start();
    //循环
    while(true) {
        std::vector<Channel*> channels;
        int fds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1); //epoll监听到有事件
        if(fds == -1) {
            std::cout << "epoll wait error， errno:" << errno << std::endl;
            break;
        }
        for(int i = 0; i < fds; i++) {
            auto* pChannel = static_cast<Channel*>(_events[i].data.ptr);
            pChannel->setRevents(_events[i].events);
            channels.push_back(pChannel);
        }
        for(auto c : channels) {
            c->handleEvent();
        }
    }
}

