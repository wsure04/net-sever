#include"TcpServer.h"
/*
class TcpSever
{
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
}
TcpServer::~TcpServer()
{
    delete acceptor_;
}

void TcpServer::start()
{
    loop_.run();
} //运行事件循环

void TcpServer::newConnection(Socket *client_sock)
{
    Connection *conn = new Connection(&loop_, client_sock);//暂时未释放
}