#include"Acceptor.h"
#include"Connection.h"

Acceptor::Acceptor(const std::unique_ptr<EventLoop> &loop, const std::string &ip, const uint16_t port)
:loop_(loop), servsock_(createNonblocking()), acceptchannel_(loop_, servsock_.fd())
{
    //serv_sock要用堆对象 在析构函数中会关闭fd
    //servsock_ = new Socket(createNonblocking());//封装好的套接字
    InetAddress serv_addr(ip, port);//封装好的地址结构
    servsock_.setReuseAddr(true);
    servsock_.setTcpNodelay(true); 
    servsock_.setReusePort(true);
    servsock_.setKeepAlive(true);
    servsock_.bind(serv_addr);
    servsock_.listen();

    //acceptchannel_ = new Channel(loop_, servsock_.fd());
    acceptchannel_.setReadCallback(std::bind(&Acceptor::newConnection, this));
    acceptchannel_.enableReading();
}
Acceptor::~Acceptor()
{
    //delete servsock_;
    //delete acceptchannel_;
}

void Acceptor::newConnection()//处理连接事件
{
    InetAddress client_addr;

    std::unique_ptr<Socket> client_sock(new Socket(servsock_.accept(client_addr)));
    client_sock->setIpPort(client_addr.ip(), client_addr.port());

    newconnectioncb_(std::move(client_sock));//回调tcpserver的函数
}

void Acceptor::set_newconnectioncb(std::function<void(std::unique_ptr<Socket>)> fn)
{
    newconnectioncb_ = fn;
}
