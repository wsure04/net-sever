#include"TcpServer.h"
/*
class TcpSever

    private:
        EventLoop loop_; //一个tcpsever可以有多个事件循环 现在是单线程 暂时只用一个
    public:
        TcpSever(const std::string &ip, uint16_t port);
        ~TcpSever();
};
*/
TcpServer::TcpServer(const std::string &ip, uint16_t port)
{
    acceptor_ = new Acceptor(&loop_, ip, port);
    acceptor_->set_newconnectioncb(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));
    loop_.setEpollwaitTimeout(std::bind(&TcpServer::epollTimeout, this, std::placeholders::_1));
}
TcpServer::~TcpServer()
{
    delete acceptor_;
    for(auto &co : conns_)
    {
        delete co.second;
    }
}

void TcpServer::start()
{
    loop_.run();
} //运行事件循环

void TcpServer::newConnection(Socket *client_sock)
{
    Connection *conn = new Connection(&loop_, client_sock);
    conn->setCloseCallback(std::bind(&TcpServer::closeConnection, this, std::placeholders::_1));   
    conn->setErrorCallback(std::bind(&TcpServer::errorConnection, this, std::placeholders::_1));   
    conn->setOnmessageCallback(std::bind(&TcpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    conn->setSendCompleteCallback(std::bind(&TcpServer::sendComplete, this, std::placeholders::_1));
    printf("新的客户端(fd:%d,ip:%s,port:%d)连接\n", conn->fd(), conn->ip().c_str(), conn->port());

    conns_[conn->fd()] = conn;
}


void TcpServer::closeConnection(Connection *conn)
{
    printf("客户端(%d)已关闭\n", conn->fd());
    //close(conn->fd());//关闭客户端
    conns_.erase(conn->fd()); //从map中删除conn
    delete conn;
}//关闭客户端连接 在Connection中回调此函数

void TcpServer::errorConnection(Connection *conn)
{
    printf("客户端(%d)错误\n", conn->fd());
    conns_.erase(conn->fd()); //从map中删除conn
    delete conn;
} 

void TcpServer::onMessage(Connection* conn, std::string message)
{
    message = "reply:" + message;

    int len = message.size();
    std::string tmpbuf((char*)&len, 4);
    tmpbuf.append(message);

    //send(conn->fd(), tmpbuf.data(), len + 4, 0);
    conn->send(tmpbuf.c_str(), tmpbuf.size());
} //处理客户端的请求报文， 在Connection中回调此函数


void TcpServer::sendComplete(Connection *conn)
{
    printf("send cmoplete.\n");

    //通知TCP发送完成 
}
//epoll_wait超时 在EventLoop中回调
void TcpServer::epollTimeout(EventLoop *loop)
{
    printf("epoll_wait() timeout.\n");

    //可以增加其他代码
}