#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"

class TcpServer
{
    private:
        EventLoop loop_; //一个tcpsever可以有多个事件循环 现在是单线程 暂时只用一个
    public:
        TcpServer(const std::string &ip, uint16_t port);
        ~TcpServer();

        void start(); //运行事件循环
};