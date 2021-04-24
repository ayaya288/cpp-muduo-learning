//
// Created by 水瀬あくあ on 2021/4/24.
//

#ifndef CPP_MUDUO_LEARNING_TCPSERVER_H
#define CPP_MUDUO_LEARNING_TCPSERVER_H

#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>


#define MAX_LINE 1024
#define MAX_EVENTS 500
#define MAX_LISTEN_FD 5
#define LISTEN_PORT 6868

#define SOCKET_CREATE_ERROR -10
#define EPOLL_ERROR -11

class TcpServer {
public:
    TcpServer();
    ~TcpServer();
    void start();
private:
    int createAndListen();

};

#endif //CPP_MUDUO_LEARNING_TCPSERVER_H
