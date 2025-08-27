#pragma once
#include"EventLoop.h"
#include"Socket.h"
#include"Channel.h"
#include"Acceptor.h"
#include"Connection.h"
#include"ThreadPool.h"
#include<map>
class TcpServer
{
    private:
        EventLoop *mainloop_; //主事件循环
        std::vector<EventLoop*> subloops_; //存放从事件循环
        Acceptor *acceptor_;
        ThreadPool *threadpool_;    //线程池
        int threadnum_; //线程池的大小 即从时间循环的个数
        std::map<int, spConnection> conns_; //一个TcpServer有多个Connection对象，存放在容器中
        std::function<void(spConnection)> newconnectioncb_;
        std::function<void(spConnection)> closeconnectioncb_;
        std::function<void(spConnection)> errorconnectioncb_;
        std::function<void(spConnection, std::string &message)> onmessagecb_;
        std::function<void(spConnection)> sendcompletecb_;
        std::function<void(EventLoop*)> timeoutcb_;
    public:
        TcpServer(const std::string &ip, uint16_t port, int threadnum = 3);
        ~TcpServer();

        void start(); //运行事件循环

        void newConnection(Socket *client_sock);//处理连接事件
        void closeConnection(spConnection conn);//关闭客户端连接 在Connection中回调此函数
        void errorConnection(spConnection conn);//关闭客户端连接 在Connection中回调此函数
        void onMessage(spConnection conn, std::string& message); //处理客户端的请求报文， 在Connection中回调此函数
        void sendComplete(spConnection conn);
        void epollTimeout(EventLoop *loop);

        void setNewConnectioncb(std::function<void(spConnection)> fn);
        void setCloseConnectioncb(std::function<void(spConnection)> fn);
        void setErrorConnectioncb(std::function<void(spConnection)> fn);
        void setOnMessagecb(std::function<void(spConnection, std::string &message)> fn);
        void setSendCompletecb(std::function<void(spConnection)> fn);
        void setTimeout(std::function<void(EventLoop*)> fn);
};