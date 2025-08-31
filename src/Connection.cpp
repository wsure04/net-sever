#include"Connection.h"

Connection::Connection(const std::unique_ptr<EventLoop>& loop, std::unique_ptr<Socket> clientsock)
:loop_(loop), clientsock_(std::move(clientsock)), disconnect_(false), clientchannel_(new Channel(loop_, clientsock_->fd()))
{
    //clientchannel_ = new Channel(loop_, clientsock_->fd());
    clientchannel_->setReadCallback(std::bind(&Connection::onMessage, this));
    clientchannel_->setCloseCallback(std::bind(&Connection::closeCallback,  this));
    clientchannel_->setErrorCallback(std::bind(&Connection::errorCallback, this));
    clientchannel_->setWriteCallback(std::bind(&Connection::writeCallback, this));
    clientchannel_->useET();
    clientchannel_->enableReading();
}
Connection::~Connection()
{
    //delete clientchannel_;
    //delete clientsock_;
    //printf("Channel对象已析构\n");
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
    disconnect_ = true;
    clientchannel_->remove();
   closecallback_(shared_from_this());
}//TCP连接关闭的回调函数， 供Channel回调

void Connection::errorCallback()
{
    disconnect_ = true;
    clientchannel_->remove();
    errorcallback_(shared_from_this());
}  //TCP连接错误的回调函数，供Channel

void Connection::writeCallback()
{
    int writen = ::send(fd(), outputbuffer_.data(), outputbuffer_.size(), 0);//尝试把发送缓冲区中的数据全部发送出去
    if(writen > 0) outputbuffer_.erase(0, writen); //从发送缓冲区中删除已经发送的数据

    //缓冲区中没有数据 发送完成 不再关注写事件 防止忙轮询
    if(outputbuffer_.size() == 0)
    {
        clientchannel_->disableWriting();
        sendcompletecallback_(shared_from_this());//发送完成通知TcpServer层
    }
} //处理写事件的回调函数 供channel回调

void Connection::setCloseCallback(std::function<void(spConnection)> fn)
{
    closecallback_ = fn;
}

void Connection::setErrorCallback(std::function<void(spConnection)> fn)
{
    errorcallback_ = fn;
}

 void Connection::setOnmessageCallback(std::function<void(spConnection, std::string&)> fn)
{
    onmessagecallback_ = fn;
}

void Connection::setSendCompleteCallback(std::function<void(spConnection)> fn)
{
    sendcompletecallback_ = fn;
}

void Connection::onMessage()//处理对端发来的消息
{
    char buf[BUFSIZ];
    //注意 使用的是非阻塞io
    while(true)
    {
        bzero(&buf, sizeof(buf));
        ssize_t readn = recv(fd(), buf, sizeof(buf), 0);
        
        if(readn > 0)//读取到了数据
        {
            inputbuffer_.append(buf, readn); //把读取的数据追加到接收缓冲区中
        }
        else if(readn == -1 && errno == EINTR) //数据读取时被信号中断 继续读取
        {
            continue;
        }
        else if(readn == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))//数据读取完毕 非阻塞立即返回跳出
        {
            while (1)
            {
                //这段代码可以封装
                //////////////////////////////////////////////////////////////
                int len;
                memcpy(&len, inputbuffer_.data(), 4);
                //如果接收缓冲区的数据量小于报文头部 说明接收缓冲区的报文信息不完整
                if(inputbuffer_.size() < len + 4) break;

                std::string message(inputbuffer_.data()+4, len);//取出报文内容
                inputbuffer_.erase(0, len+4);
                ////////////////////////////////////////////////////////////

                printf("收到(fd:%d)的报文:%s\n", fd(), message.c_str());

                //假设经过了一些计算 计算不应该在底层类
               onmessagecallback_(shared_from_this(), message); //调用TcpServer::onMessage()
            }
            break;
        }
        else if(readn == 0)
        {
            //clientchannel_->remove();
            closeCallback(); //调用TcpServer::closeCallback()
            break;
        }
    }
}


void Connection::send(const char* data, size_t size)
{
    if(disconnect_ == true)
    {
        printf("客户端连接已断开， 数据未发送。\n");
        return;
    }
    outputbuffer_.appendWithHead(data, size);
    clientchannel_->enableWriting();//注册写事件
}




