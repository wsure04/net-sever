#pragma once
#include<functional>
#include"Socket.h"
#include"InetAddress.h"
#include"Channel.h"
#include"EventLoop.h"
#include"Buffer.h"
#include<memory>
#include<atomic>

class Connection;
using spConnection = std::shared_ptr<Connection>;

class Connection : public std::enable_shared_from_this<Connection>
{
    private:
        EventLoop *loop_; //事件循环 在构造函数中传入。
        Socket *clientsock_;
        Channel *clientchannel_;

        Buffer inputbuffer_; //接收缓冲区
        Buffer outputbuffer_; //发送缓冲区

        std::atomic_bool disconnect_;//在io线程中会改变这个值 如果已经断开设置为true

        std::function<void(spConnection)> closecallback_;
        std::function<void(spConnection)> errorcallback_;
        std::function<void(spConnection, std::string&)> onmessagecallback_;
        std::function<void(spConnection)> sendcompletecallback_;
    public:
        Connection(EventLoop *loop, Socket *clientsock);
        ~Connection();

        int fd() const;//返回fd成员
        std::string ip() const;
        uint16_t port() const;

        void closeCallback();   //TCP连接关闭的回调函数， 供Channel回调
        void errorCallback();   //TCP连接错误的回调函数，供Channel
        void onMessage();//处理对端发来的消息
        void writeCallback(); //处理写事件的回调函数 供channel回调

        void setCloseCallback(std::function<void(spConnection)> fn);

        void setErrorCallback(std::function<void(spConnection)> fn);

        void setOnmessageCallback(std::function<void(spConnection, std::string&)> fn);

        void send(const char* data, size_t size);

        void setSendCompleteCallback(std::function<void(spConnection)> fn);
};