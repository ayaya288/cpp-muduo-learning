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
    _sockfd(sockfd) {
    //TODO:内存泄漏
    _pChannel = new Channel(_loop, sockfd);
    _pChannel->setCallBack(this);
    _pChannel->enableReading();
}

TcpConnection::~TcpConnection() = default;

void TcpConnection::OnIn(int sockfd) {
    //处理业务
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