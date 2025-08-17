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
    clientchannel_->setCallback(std::bind(&Channel::onMessage, clientchannel_));
    clientchannel_->useET();
    clientchannel_->enableReading();
}
Connection::~Connection()
{
    delete clientchannel_;
    //Channel类中创建了clientsock之后并没有释放内存，但是Connection知道释放时机
    delete clientsock_;
}