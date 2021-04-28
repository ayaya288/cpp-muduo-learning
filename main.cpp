//
// Created by 水瀬あくあ on 2021/4/24.
//

#include "EchoServer.h"
#include "EventLoop.h"

int main() {
    EventLoop loop;
    EchoServer echoServer(&loop);
    echoServer.start();
    loop.loop();
    return 0;
}