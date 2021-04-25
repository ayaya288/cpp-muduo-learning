//
// Created by 水瀬あくあ on 2021/4/24.
//

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include <vector>

#include "TcpServer.h"
#include "Channel.h"

TcpServer::TcpServer():
    _epollfd(-1),
    _listenfd(-1) {}

TcpServer::~TcpServer() = default;

int TcpServer::createAndListen() {
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

void TcpServer::OnIn(int sockfd) {
    std::cout << "OnIn:" << sockfd << std::endl;
    if(sockfd == _listenfd) { //有新的连接请求
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

        auto* pChannel = new Channel(_epollfd, connfd);
        pChannel->setCallBack(this);
        pChannel->enableReading();

    } else {
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
            if(write(sockfd, line, read_length) != read_length) {
                std::cout << "error: write incomplete" << std::endl;
            }
        }
    }
}

void TcpServer::start() {
    //创建epoll
    _epollfd = epoll_create(1);
    if(_epollfd <= 0) {
        std::cout << "epoll error, error:" << _epollfd << std::endl;
        exit(EPOLL_ERROR);
    }
    //将监听套接字加入EPOLL_IN
    _listenfd = createAndListen();
    auto* pChannel = new Channel(_epollfd, _listenfd);
    pChannel->setCallBack(this);
    pChannel->enableReading();

    //循环
    while(true) {
        std::vector<Channel*> channels;
        int fds = epoll_wait(_epollfd, _events, MAX_EVENTS, -1); //epoll监听到有事件
        if(fds == -1) {
            std::cout << "epoll wait error:" << fds << std::endl;
            break;
        }
        for(int i = 0; i < fds; i++) {
            pChannel = static_cast<Channel*>(_events[i].data.ptr);
            pChannel->setRevents(_events[i].events);
            channels.push_back(pChannel);
        }
        for(auto c : channels) {
            c->handleEvent();
        }
    }
}

