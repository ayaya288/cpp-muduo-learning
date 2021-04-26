//
// Created by 水瀬あくあ on 2021/4/24.
//

#include "TcpServer.h"
#include "EventLoop.h"

int main() {
    EventLoop loop;
    TcpServer tcpServer(&loop);
    tcpServer.start();
    loop.loop();

    return 0;
}