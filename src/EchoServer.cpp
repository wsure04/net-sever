#include"EchoServer.h"

/*
class EchoServer
{
    private:
        TcpServer tcpserver_;

    public:
        EchoServer(const std::string &ip, uint16_t port);
       ~EchoServer();

        void Start();

        void handleNewConnection(Socket *client_sock);//处理新客户端连接请求
        void handleClose(Connection *conn);//关闭客户端连接 
        void handleError(Connection *conn);//客户端连接错误
        void handleMessage(Connection* conn, std::string message); //处理客户端的请求报文
        void handleSendComplete(Connection *conn);//数据发送完成
        void handleTimeout(EventLoop *loop);//epoll_event()超时
        //事件处理函数将在TcpServer类中回调
};
*/
EchoServer::EchoServer(const std::string &ip, uint16_t port, int subthreadnum, int worknum):tcpserver_(ip, port, subthreadnum), threadpool_(worknum, "Work")
{
    tcpserver_.setNewConnectioncb(std::bind(&EchoServer::handleNewConnection, this, std::placeholders::_1));
    tcpserver_.setCloseConnectioncb(std::bind(&EchoServer::handleClose, this, std::placeholders::_1));
    tcpserver_.setErrorConnectioncb(std::bind(&EchoServer::handleError, this, std::placeholders::_1));
    tcpserver_.setOnMessagecb(std::bind(&EchoServer::handleMessage, this, std::placeholders::_1, std::placeholders::_2));
    tcpserver_.setSendCompletecb(std::bind(&EchoServer::handleSendComplete, this, std::placeholders::_1));
    tcpserver_.setTimeout(std::bind(&EchoServer::handleTimeout, this, std::placeholders::_1));
}
EchoServer::~EchoServer()
{

}

void EchoServer::Start()
{
    tcpserver_.start();
}

void EchoServer::handleNewConnection(spConnection conn)//处理新客户端连接请求
{
    std::cout << "New Connection Come in." << std::endl;
    //printf("EchoServe::handleNewConnection() thread is %d.\n", syscall(SYS_gettid));
    //可以增加业务代码
}
void EchoServer::handleClose(spConnection conn)//关闭客户端连接 
{
    std::cout << "EchoServer Connection close." << std::endl;
}
void EchoServer::handleError(spConnection conn)//客户端连接错误
{
    std::cout << "EchoServe Connection Error." << std::endl;
}
void EchoServer::handleMessage(spConnection conn, std::string &message)//处理客户端的请求报文
{
    //printf("EchoServer::handleMessage() thread is %d.\n", syscall(SYS_gettid));
    //把业务添加到线程池的任务队列
    threadpool_.addtask(std::bind(&EchoServer::onMessage, this, conn, message));

}
void EchoServer::handleSendComplete(spConnection conn)//数据发送完成
{
    std::cout << "Message Send Complete." << std::endl;
}
void EchoServer::handleTimeout(EventLoop *loop)//epoll_event()超时
{
    std::cout << "EchoServer Timeout." << std::endl;
}

void EchoServer::onMessage(spConnection conn, std::string &message)
{
    message = "reply-" + message;

    //send(conn->fd(), tmpbuf.data(), len + 4, 0);
    conn->send(message.data(), message.size());
}