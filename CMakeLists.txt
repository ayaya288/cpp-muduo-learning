cmake_minimum_required(VERSION 3.19)
project(cpp-muduo-learning)

set(CMAKE_CXX_STANDARD 14)

add_executable(cpp-muduo-learning
        IAcceptorCallBack.h IChannelCallBack.h IMuduoUser.h IRun.h
        main.cpp Declear.h Define.h
        Acceptor.cpp Acceptor.h
        BlockingQueue.h
        Buffer.cpp Buffer.h
        Condition.h
        Channel.cpp Channel.h
        EchoServer.cpp EchoServer.h
        Epoll.cpp Epoll.h
        EventLoop.cpp EventLoop.h
        Mutex.h
        Task.cpp Task.h
        TcpConnection.cpp TcpConnection.h
        TcpServer.cpp TcpServer.h
        Thread.cpp Thread.h
        ThreadPool.cpp ThreadPool.h
        Timer.h
        TimeStamp.cpp TimeStamp.h
        TimerQueue.cpp TimerQueue.h)

find_package(Threads REQUIRED)
target_link_libraries(cpp-muduo-learning Threads::Threads)