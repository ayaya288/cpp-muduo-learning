//
// Created by 水瀬あくあ on 2021/4/24.
//

#include "TcpServer.h"

TcpServer::TcpServer() = default;

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

void TcpServer::start() {
    //创建并初始化需要用到的变量
    struct epoll_event event, events[MAX_EVENTS];
    bzero(&event, sizeof(epoll_event));
    bzero(events, sizeof(events));
    int listen_fd, conn_fd, sock_fd;
    int read_length;
    char line[MAX_LINE];
    struct sockaddr_in client_addr;
    bzero(&client_addr, sizeof(client_addr));
    socklen_t client_len = sizeof(struct sockaddr_in);

    //创建epoll
    int epoll_fd = epoll_create(1);
    if(epoll_fd < 0) {
        std::cout << "epoll error, error:" << epoll_fd << std::endl;
        exit(EPOLL_ERROR);
    }
    //将监听套接字加入EPOLL_IN
    listen_fd = createAndListen();
    event.data.fd = listen_fd;
    event.events = EPOLLIN;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);

    //循环
    while(true) {
        int fds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1); //epoll监听到有事件
        if(fds == -1) {
            std::cout << "epoll wait error:" << fds << std::endl;
            break;
        }
        for(int i = 0; i < fds; i++) {
            if(events[i].data.fd == listen_fd) { //有新的连接请求
                conn_fd = accept(listen_fd, (sockaddr*)&client_addr, (socklen_t*)&client_len);
                if(conn_fd > 0) {
                    std::cout << "New Connection from: ["
                              << inet_ntoa(client_addr.sin_addr) << ':'
                              << ntohs(client_addr.sin_port) <<']'
                              << " accepted, Socket ID: "
                              << conn_fd
                              << std::endl;
                } else {
                    std::cout << "accept error, errno" << errno << std::endl;
                }
                fcntl(conn_fd, F_SETFL, O_NONBLOCK);
                event.data.fd = conn_fd;
                event.events = EPOLLIN;
                if(-1 == epoll_ctl(epoll_fd, EPOLL_CTL_ADD, conn_fd, &event)) {
                    std::cout << "epoll add new event error, errno: " << errno <<  std::endl;
                }
            } else if(events[i].events & EPOLLIN) { //新的传入事件,做出处理
                sock_fd = events[i].data.fd;
                if(sock_fd < 0) {
                    std::cout << "socket id minus error, errno: "<< errno << std::endl;
                    continue;
                }
                // 开始处理输入的数据
                bzero(line, MAX_LINE);
                read_length = read(sock_fd, line, MAX_LINE);
                if(read_length < 0) {
                    if(errno == ECONNRESET) {
                        std::cout << "ECONNRESET error, closed: " << conn_fd << std::endl;
                        close(conn_fd);
                    }
                } else if(read_length == 0) {
                    std::cout << "Client Connection closed: " << conn_fd << std::endl;
                    close(conn_fd);
                } else {
                    if(write(sock_fd, line, read_length) != read_length) {
                        std::cout << "error: write incomplete" << std::endl;
                    }
                }
            }
        }
    }
}

