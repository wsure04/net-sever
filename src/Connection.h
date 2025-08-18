#pragma once
#include<functional>
#include"Socket.h"
#include"InetAddress.h"
#include"Channel.h"
#include"EventLoop.h"

class Connection
{
    private:
        EventLoop *loop_; //事件循环 在构造函数中传入。
        Socket *clientsock_;
        Channel *clientchannel_;
    public:
        Connection(EventLoop *loop, Socket *clientsock);
        ~Connection();

        int fd() const;//返回fd成员
        std::string ip() const;
        uint16_t port() const;

        void closeCallback();   //TCP连接关闭的回调函数， 供Channel回调
        void errorCallback();   //TCP连接错误的回调函数，供Channel
};