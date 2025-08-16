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
    //serv_sock要用堆对象 在析构函数中会关闭fd
    Socket *serv_sock = new Socket(createNonblocking());//封装好的套接字
    InetAddress serv_addr(ip, port);//封装好的地址结构
    //设置sock属性
    const bool on = true;
    serv_sock->setReuseAddr(on);
    serv_sock->setReusePort(on);
    serv_sock->setTcpNodelay(on);
    serv_sock->setKeepAlive(on);
    //绑定地址结构
    serv_sock->bind(serv_addr);
    //设置监听上限
    serv_sock->listen();

    //创建epoll句柄

    Channel* serv_channel = new Channel(&loop_, serv_sock->fd());
    serv_channel->setCallback(std::bind(&Channel::newConnection, serv_channel, serv_sock));
    serv_channel->enableReading();
}
TcpServer::~TcpServer()
{

}

void TcpServer::start()
{
    loop_.run();
} //运行事件循环