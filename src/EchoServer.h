#pragma once
#include"TcpServer.h"
#include"EventLoop.h"
#include"Connection.h"
#include"ThreadPool.h"
class EchoServer
{
    private:
        TcpServer tcpserver_;

        ThreadPool threadpool_;//工作线程
    public:
        EchoServer(const std::string &ip, const uint16_t port, int subthreadnum = 3, int worknum = 5);
        ~EchoServer();

        void Start();

        void handleNewConnection(spConnection conn);//处理新客户端连接请求
        void handleClose(spConnection conn);//关闭客户端连接 
        void handleError(spConnection conn);//客户端连接错误
        void handleMessage(spConnection conn, std::string &message); //处理客户端的请求报文
        void handleSendComplete(spConnection conn);//数据发送完成
        void handleTimeout(EventLoop *loop);//epoll_event()超时
        //事件处理函数将在TcpServer类中回调
        void onMessage(spConnection conn, std::string &message);
};