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
    public:
        Acceptor(EventLoop *loop, const std::string &ip, uint16_t port);
        ~Acceptor();
};