//
// Created by 水瀬あくあ on 2021/4/26.
//

#include "Acceptor.h"
#include "Define.h"

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cstring>

Acceptor::Acceptor(int epollfd):
        _epollfd(epollfd),
        _listenfd(-1),
        _pAcceptChannel(nullptr),
        _pCallBack(nullptr) {}

Acceptor::~Acceptor() = default;

void Acceptor::OnIn(int sockfd) {
    //处理新的连接请求
    int connfd;
    struct sockaddr_in client_addr{};
    socklen_t client_len = sizeof(struct sockaddr_in);
    connfd = accept(_listenfd, (sockaddr*)&client_addr, (socklen_t*)&client_len);
    if(connfd > 0) {
        std::cout << "New Connection from: ["
                  << inet_ntoa(client_addr.sin_addr) << ':'
                  << ntohs(client_addr.sin_port) <<']'
                  << " accepted, Socket ID: "
                  << connfd
                  << std::endl;
    } else {
        std::cout << "accept error, errno" << errno << std::endl;
    }
    fcntl(connfd, F_SETFL, O_NONBLOCK);

    _pCallBack->newConnection(connfd);
}

void Acceptor::setCallBack(IAcceptorCallBack *pCallBack) {
    _pCallBack = pCallBack;
}

void Acceptor::start() {
    _listenfd = createAndListen();
    //TODO:内存泄漏
    _pAcceptChannel = new Channel(_epollfd, _listenfd);
    _pAcceptChannel->setCallBack(this);
    _pAcceptChannel->enableReading();
}

int Acceptor::createAndListen() {
    //用于创建Socket,返回监听用套接字
    int on = 1;
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(struct sockaddr_in));
    //非阻塞IO
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(LISTEN_PORT);

    if(-1 == bind(listen_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in))) {
        std::cout << "bind error, errno:" << errno << std::endl;
        exit(SOCKET_CREATE_ERROR);
    }
    if(-1 == listen(listen_fd, MAX_LISTEN_FD)) {
        std::cout << "listen error, errno:" << errno << std::endl;
        exit(SOCKET_CREATE_ERROR);
    }

    return listen_fd;
}