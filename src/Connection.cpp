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
    clientchannel_->setReadCallback(std::bind(&Connection::onMessage, this));
    clientchannel_->setCloseCallback(std::bind(&Connection::closeCallback, this));
    clientchannel_->setErrorCallback(std::bind(&Connection::errorCallback, this));
    clientchannel_->setWriteCallback(std::bind(&Connection::writeCallback, this));
    clientchannel_->useET();
    clientchannel_->enableReading();
}
Connection::~Connection()
{
    delete clientchannel_;
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

void Connection::writeCallback()
{
    int writen = ::send(fd(), outputbuffer_.data(), outputbuffer_.size(), 0);//尝试把发送缓冲区中的数据全部发送出去
    if(writen > 0) outputbuffer_.erase(0, writen); //从发送缓冲区中删除已经发送的数据

    //缓冲区中没有数据 发送完成 不再关注写事件 防止忙轮询
    if(outputbuffer_.size() == 0)
    {
        clientchannel_->disableWriting();
        sendcompletecallback_(this);//发送完成通知TcpServer层
    }
} //处理写事件的回调函数 供channel回调

void Connection::setCloseCallback(std::function<void(Connection*)> fn)
{
    closecallback_ = fn;
}

void Connection::setErrorCallback(std::function<void(Connection*)> fn)
{
    errorcallback_ = fn;
}

 void Connection::setOnmessageCallback(std::function<void(Connection*, std::string&)> fn)
{
    onmessagecallback_ = fn;
}

void Connection::setSendCompleteCallback(std::function<void(Connection*)> fn)
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
            //printf("接收到数据(来自:%d)：%s\n", fd(), buf);
            //send(fd(), buf, strlen(buf), 0);
            //接收到数据之后，先不发送 现放到接收缓冲区
            inputbuffer_.append(buf, readn); //把读取的数据追加到接收缓冲区中
        }
        else if(readn == -1 && errno == EINTR) //数据读取时被信号中断 继续读取
        {
            continue;
        }
        else if(readn == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))//数据读取完毕 非阻塞立即返回跳出
        {
            //接收完成
            //printf("接收到数据(来自:%d)：%s\n", fd(), inputbuffer_.data());
            //在这里进行计算
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
               onmessagecallback_(this, message); //调用TcpServer::onMessage()
            }
            break;
        }
        else if(readn == 0)
        {    
            closeCallback(); //调用TcpServer::closeCallback()
            break;
        }
    }
}


void Connection::send(const char* data, size_t size)
{
    outputbuffer_.appendWithHead(data, size);
    clientchannel_->enableWriting();//注册写事件
}




