#include"Connection.h"
/*
class Connection
{
    private:
        EventLoop *loop_; //事件循环 在构造函数中传入。
        Socket *clientsock_;
        Channel *clientchannel_;
    public:
        Connection(EventLoop *loop, Socket *clientsock);
        ~Connection();
};
*/
Connection::Connection(EventLoop *loop, Socket *clientsock):loop_(loop), clientsock_(clientsock)
{
    clientchannel_ = new Channel(loop_, clientsock_->fd());
    clientchannel_->setReadCallback(std::bind(&Channel::onMessage, clientchannel_));
    clientchannel_->setCloseCallback(std::bind(&Connection::closeCallback, this));
    clientchannel_->setErrorCallback(std::bind(&Connection::errorCallback, this));
    clientchannel_->useET();
    clientchannel_->enableReading();
}
Connection::~Connection()
{
    delete clientchannel_;
    //Channel类中创建了clientsock之后并没有释放内存，但是Connection知道释放时机
    delete clientsock_;
}

int Connection::fd() const
{
    return clientsock_->fd();
}//返回fd成员
std::string Connection::ip() const
{
    return clientsock_->ip();
}
uint16_t Connection::port() const
{
    return clientsock_->port();
}

void Connection::closeCallback()
{
   closecallback_(this);
}//TCP连接关闭的回调函数， 供Channel回调

void Connection::errorCallback()
{
    errorcallback_(this);
}  //TCP连接错误的回调函数，供Channel

void Connection::setCloseCallback(std::function<void(Connection*)> fn)
{
    closecallback_ = fn;
}

void Connection::setErrorCallback(std::function<void(Connection*)> fn)
{
    errorcallback_ = fn;
}
