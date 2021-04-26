cmake_minimum_required(VERSION 3.19)
project(cpp-muduo-learning)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp-muduo-learning
        main.cpp Declear.h Define.h
        IChannelCallBack.h
        TcpServer.cpp TcpServer.h
        Channel.cpp Channel.h
        IAcceptorCallBack.h
        TcpConnection.cpp TcpConnection.h
        Acceptor.cpp Acceptor.h
        EventLoop.cpp EventLoop.h
        Epoll.cpp Epoll.h)
