# cpp-muduo-learning
学习使用cpp实现mini-muduo网络库的编程
****
* ###v0.1 实现基本的epoll  
  1. 在一个main.cpp实现listenfd的创建监听，实现epoll的创建（监听端口6868）
* ###v0.2 改写main函数为一个TcpServer类
  1. 将main.cpp拆分成main.cpp和TcpServer.cpp，main函数仅保留创建TcpServer类  
  2. 主要逻辑移至TcpServer类中，与0.1版本基本保持不变
* ###v0.3 加入Channel类
  1. 引入Channel类，每一个Channel对象负责一个fd的处理  
  2. TcpServer类中，新增私有成员变量保存listenfd与epollfd  
  3. 新增map<int, Channel*>记录fd与Channel对应关系  
  4. Channel类中的_callBack保存了一个实现了IChannelCallBack接口的对象（TcpServer）
  5. 利用epoll_data保存Channel的指针，当监听到epoll事件时，可以直接获得Channel对象的指针
    