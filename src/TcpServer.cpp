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
TcpServer::TcpServer(const std::string &ip, uint16_t port, int threadnum):threadnum_(threadnum)
{
    mainloop_ = new EventLoop;//创建主事件循环
    mainloop_->setEpollwaitTimeout(std::bind(&TcpServer::epollTimeout, this, std::placeholders::_1));

    acceptor_ = new Acceptor(mainloop_, ip, port);
    acceptor_->set_newconnectioncb(std::bind(&TcpServer::newConnection, this, std::placeholders::_1));

    threadpool_ = new ThreadPool(threadnum_, "I/O"); //创建线程池

    //创建从事件循环
    for(int i = 0; i < threadnum_; i++)
    {
        subloops_.push_back(new EventLoop);          //创建事件循环 放入容器中
        subloops_[i]->setEpollwaitTimeout(std::bind(&TcpServer::epollTimeout, this, std::placeholders::_1));
        threadpool_->addtask(std::bind(&EventLoop::run, subloops_[i]));//把事件循环的run函数作为任务传给线程池
    }
}
TcpServer::~TcpServer()
{
    delete acceptor_;
    delete mainloop_;

    for(auto &co : conns_)
    {
        delete co.second;
    }

    for(auto &loop : subloops_)
    {
        delete loop;
    }

    delete threadpool_; //释放线程池
}

void TcpServer::start()
{
    mainloop_->run();
} //运行事件循环

void TcpServer::newConnection(Socket *client_sock)
{
    //把新建的conn分配给从事件循环
    Connection *conn = new Connection(subloops_[client_sock->fd()%threadnum_], client_sock);
    conn->setCloseCallback(std::bind(&TcpServer::closeConnection, this, std::placeholders::_1));   
    conn->setErrorCallback(std::bind(&TcpServer::errorConnection, this, std::placeholders::_1));   
    conn->setOnmessageCallback(std::bind(&TcpServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
    conn->setSendCompleteCallback(std::bind(&TcpServer::sendComplete, this, std::placeholders::_1));
    //printf("新的客户端(fd:%d,ip:%s,port:%d)连接\n", conn->fd(), conn->ip().c_str(), conn->port());

    conns_[conn->fd()] = conn;

    if(newconnectioncb_) newconnectioncb_(conn);//回调EchoServer::handleNewConnection
}


void TcpServer::closeConnection(Connection *conn)
{
    if(closeconnectioncb_) closeconnectioncb_(conn);

    //printf("客户端(%d)已关闭\n", conn->fd());
    //close(conn->fd());//关闭客户端
    conns_.erase(conn->fd()); //从map中删除conn
    delete conn;
}//关闭客户端连接 在Connection中回调此函数

void TcpServer::errorConnection(Connection *conn)
{
    if(errorconnectioncb_) errorconnectioncb_(conn);

    //printf("客户端(%d)错误\n", conn->fd());
    conns_.erase(conn->fd()); //从map中删除conn
    delete conn;
} 

void TcpServer::onMessage(Connection* conn, std::string &message)
{
    if(onmessagecb_) onmessagecb_(conn, message);
} //处理客户端的请求报文， 在Connection中回调此函数


void TcpServer::sendComplete(Connection *conn)
{
   //printf("send cmoplete.\n");

    if(sendcompletecb_) sendcompletecb_(conn);
}
//epoll_wait超时 在EventLoop中回调
void TcpServer::epollTimeout(EventLoop *loop)
{
    //printf("epoll_wait() timeout.\n");

    if(timeoutcb_) timeoutcb_(loop);
}
void TcpServer::setNewConnectioncb(std::function<void(Connection*)> fn)
{
    newconnectioncb_ = fn;
}
void TcpServer::setCloseConnectioncb(std::function<void(Connection*)> fn)
{
    closeconnectioncb_ = fn;
}
void TcpServer::setErrorConnectioncb(std::function<void(Connection*)> fn)
{
    errorconnectioncb_ = fn;
}
void TcpServer::setOnMessagecb(std::function<void(Connection*, std::string &message)> fn)
{
    onmessagecb_ = fn;
}
void TcpServer::setSendCompletecb(std::function<void(Connection*)> fn)
{
    sendcompletecb_ = fn;
}
void TcpServer::setTimeout(std::function<void(EventLoop*)> fn)
{
    timeoutcb_ = fn;
}