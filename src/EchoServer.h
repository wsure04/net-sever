#pragma once
#include"TcpServer.h"
#include"EventLoop.h"
#include"Connection.h"

class EchoServer
{
    private:
        TcpServer tcpserver_;

    public:
        EchoServer(const std::string &ip, uint16_t port);
        ~EchoServer();

        void Start();

        void handleNewConnection(Connection* conn);//处理新客户端连接请求
        void handleClose(Connection *conn);//关闭客户端连接 
        void handleError(Connection *conn);//客户端连接错误
        void handleMessage(Connection* conn, std::string &message); //处理客户端的请求报文
        void handleSendComplete(Connection *conn);//数据发送完成
        void handleTimeout(EventLoop *loop);//epoll_event()超时
        //事件处理函数将在TcpServer类中回调
};