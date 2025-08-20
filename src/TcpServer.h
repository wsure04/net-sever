#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"Acceptor.h"
#include"Connection.h"
#include<map>
class TcpServer
{
    private:
        EventLoop loop_; //一个tcpsever可以有多个事件循环 现在是单线程 暂时只用一个
        Acceptor *acceptor_;
        std::map<int, Connection*> conns_; //一个TcpServer有多个Connection对象，存放在容器中
    public:
        TcpServer(const std::string &ip, uint16_t port);
        ~TcpServer();

        void start(); //运行事件循环

        void newConnection(Socket *client_sock);//处理连接事件

        void closeConnection(Connection *conn);//关闭客户端连接 在Connection中回调此函数

        void errorConnection(Connection *conn);//关闭客户端连接 在Connection中回调此函数

        void onMessage(Connection* conn, std::string message); //处理客户端的请求报文， 在Connection中回调此函数
};