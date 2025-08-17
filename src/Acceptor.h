#pragma once
#include<functional>
#include"Socket.h"
#include"InetAddress.h"
#include"Channel.h"
#include"EventLoop.h"

class Acceptor
{
    private:
        EventLoop *loop_; //对应Acceptor事件循环 在构造函数中传入。
        Socket *servsock_;
        Channel *acceptchannel_;
        std::function<void(Socket*)> newconnectioncb_;//处理新客户端连接请求的回调函数， 将指向TcpServer::newconnection()
    public:
        Acceptor(EventLoop *loop, const std::string &ip, uint16_t port);
        ~Acceptor();
        void newConnection();//处理连接事件

        void set_newconnectioncb(std::function<void(Socket*)> fn);//设置处理新客户端连接请求的回调函数
};