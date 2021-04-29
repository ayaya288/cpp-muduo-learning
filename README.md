# cpp-muduo-learning
学习使用cpp实现mini-muduo网络库的编程
****
### v0.1 实现基本的epoll  
1. 在一个main.cpp实现listenfd的创建监听，实现epoll的创建（监听端口6868）
### v0.2 改写main函数为一个TcpServer类
1. 将main.cpp拆分成main.cpp和TcpServer.cpp，main函数仅保留创建TcpServer类
2. 主要逻辑移至TcpServer类中，与0.1版本基本保持不变
### v0.3 加入Channel类
1. 引入Channel类，每一个Channel对象负责一个fd的处理
2. TcpServer类中，新增私有成员变量保存listenfd与epollfd
3. 新增map<int, Channel*>记录fd与Channel对应关系
4. Channel类中的_callBack保存了一个实现了IChannelCallBack接口的对象（TcpServer）
5. 利用epoll_data保存Channel的指针，当监听到epoll事件时，可以直接获得Channel对象的指针
### v0.4 加入Acceptor和TcpConnection类
1. 将TcpServer拆分为Acceptor和TcpConnection类，保留主循环，管理epollfd，Map更换为存储TcpConnection的指针
2. 实现IAcceptionCallBack接口，当Acceptor处理完新连接加入epoll后，由TcpServer负责创建TcpConnection对象并记录其指针
3. Acceptor与TcpConnection实现IChannelCallBack接口，主循环之后Channel回调对应的对象，无须判断listenfd
4. Acceptor对象负责创建listenfd与处理新连接，初始化时从TcpServer获得epollfd
5. TcpConnection对象负责处理实际的业务
6. //TODO:四处内存泄漏未处理
### v0.5 加入EventLoop和Epoll类
1. 将epoll的创建，epoll_wait从TcpServer移除，包装为一个Epoll类
2. 将Channel内的Update也移至Epoll内，为Channel类增加获取Events和Sockfd的方法
3. TcpServer的start方法不再包含循环，仅仅只创建Acceptor
4. 循环新建EventLoop类管理，EventLoop类与Epoll类一起完成epoll的管理
5. 循环由main函数调用EventLoop.loop()开启，EventLoop类预留了_quit
6. 程序的结构变为，EventLoop管理Epoll循环，TcpServer管理业务逻辑，因此原本需要传递给Channel
的epollfd全部变更为EventLoop对象指针 
7. //TODO:新增一处内存泄漏
### v0.6 加入IMuduoUser接口和EchoServer类
1. 创建EchoServer类，实现IMuduoUser用于实现用户相关的逻辑处理
2. 分离TcpConnection中OnIn处理逻辑，变为调用OnMessage将数据交由EchoServer处理
3. 封装Write为send方法
4. 运行逻辑修改为main创建EchoServer对象，再由其创建TcpServer
### v0.7 加入缓冲区
1. TcpConnection加入输入输出缓冲区_inBuf, _outBuf
2. IChannelCallaBack改为handleWrite和handleRead两个接口
3. handleWrite用于处理缓冲区的数据
4. 需要增加注册EPOLLOUT事件
