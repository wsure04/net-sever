#include"Acceptor.h"
#include"Connection.h"
/*
class Acceptor
{
    private:
        EventLoop *loop_; //对应Acceptor事件循环 在构造函数中传入。
        Socket *servsock_;
        Channel *acceptchannel_;
    public:
        Acceptor(EventLoop *loop, const std::string &ip, uint16_t port);
        ~Acceptor();
};
*/

Acceptor::Acceptor(EventLoop *loop, const std::string &ip, uint16_t port):loop_(loop)
{
    //serv_sock要用堆对象 在析构函数中会关闭fd
    servsock_ = new Socket(createNonblocking());//封装好的套接字
    InetAddress serv_addr(ip, port);//封装好的地址结构
    //设置sock属性
    const bool on = true;
    servsock_->setReuseAddr(on);
    servsock_->setReusePort(on);
    servsock_->setTcpNodelay(on);
    servsock_->setKeepAlive(on);
    //绑定地址结构
    servsock_->bind(serv_addr);
    //设置监听上限
    servsock_->listen();

    //创建epoll句柄

    acceptchannel_ = new Channel(loop_, servsock_->fd());
    acceptchannel_->setCallback(std::bind(&Acceptor::newConnection, this));
    acceptchannel_->enableReading();
}
Acceptor::~Acceptor()
{
    //只有这两个成员是Accept类的真正成员
    delete servsock_;
    delete acceptchannel_;
}

void Acceptor::newConnection()//处理连接事件
{
    InetAddress client_addr;
            //接收客户端socket
    Socket *client_sock = new Socket(servsock_->accept(client_addr));

    //printf("客户端(fd:%d, ip:%s, port:%d)连接\n", client_sock->fd(), client_addr.ip(), client_addr.port());
    //Connection *conn = new Connection(loop_, client_sock);//暂时添加到这里
    newconnectioncb_(client_sock);
}

void Acceptor::set_newconnectioncb(std::function<void(Socket*)> fn)
{
    newconnectioncb_ = fn;
}
