#include"Channel.h"

/*
class Channel//将channel的地址作为epoll携带的数据
{
    private:
        int fd_ = -1; //一对一
        Epoll *ep = nullptr;//channel与epoll多对一 一个channel对应一个epoll
        bool inepoll_ = false;//channel是否已经添加到红黑树上 如果已经添加 用ADD 否则用MOD
        uint32_t events_ = 0;//fd_需要监视的事件
        uint32_t revents_ = 0;//fd已经发生的事件
    public:
        Channel(Epoll*ep, int fd);
        ~Channel();

        int fd();
        void useET();//设置采用边缘触发
        void enableReading(); //让epoll_wait监听fd_的读事件
        void setInepoll(); //设置inepoll未true
        void setRevents(uint32_t ev);//设置revents成员函数
        bool inpoll();
        uint32_t events();
        uint32_t revents();
};
*/

Channel::Channel(EventLoop *loop, int fd):loop_(loop), fd_(fd){}//初始化成员变量
Channel::~Channel()
{
    //在析构函数中 不要销毁ep_ fd_ 因为这两个东西不属于Channel类 只是需要他们
    //避免多次释放
}

int Channel::fd()//返回fd
{
    return fd_;   
}

void Channel::useET()//设置采用边缘触发
{
    events_ = events_ | EPOLLET;
}

void Channel::enableReading()//让epoll_wait监听fd_的读事件
{
    events_= events_ | EPOLLIN;
    //loop_->ep()->updateChannel(this);//将自己挂到树上   
    loop_->updateChannel(this);
}

void Channel::setInepoll() //设置inepoll未true
{
    inepoll_ = true;
}

void Channel::setRevents(uint32_t ev)//设置revents成员函数
{
    revents_ = ev;
}

bool Channel::inpoll()//返回inepoll
{
    return inepoll_;   
}

uint32_t Channel::events()//返回要监听的事件
{
    return events_;
}

uint32_t Channel::revents()//返回监听成功的事件
{
    return revents_;
}


void Channel::handleEvent()//事件处理函数
{
     //处理读写事件
    if(revents_ & EPOLLRDHUP)
    {
        printf("客户端(%d)已关闭\n", fd_);
        close(fd_);//关闭客户端
    }
    else if(revents_ & EPOLLIN|EPOLLPRI)
    {
        readcallback_();
    }
    else if(revents_ & EPOLLOUT)
    {

    }
    else//其他是为错误
    {
        printf("client(%d) error\n", fd_);
        close(fd_);
    }
}//事件处理函数 epoll_wait()返回的时候，执行它

void Channel::newConnection(Socket* serv_sock)//处理连接事件
{
    InetAddress client_addr;
            //接收客户端socket
    Socket *client_sock = new Socket(serv_sock->accept(client_addr));

    printf("客户端(fd:%d, ip:%s, port:%d)连接\n", client_sock->fd(), client_addr.ip(), client_addr.port());

    Channel* client_channel = new Channel(loop_, client_sock->fd());
    client_channel->setCallback(std::bind(&Channel::onMessage, client_channel));
    client_channel->useET();
    client_channel->enableReading();
}

void Channel::onMessage()//处理对端发来的消息
{
    char buf[BUFSIZ];
    //注意 使用的是非阻塞io
    int readn = 0;
    while(true)
    {
        bzero(buf, sizeof(buf));
        readn = recv(fd_, buf, sizeof(buf), 0);
        if(readn == 0)
        {    
            printf("客户端(%d)已关闭\n", fd_);
            close(fd_);//关闭客户端
        }
        else if(readn == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))//数据读取完毕 非阻塞立即返回跳出
        {
            break;
        }
        else if(readn == -1 && errno == EINTR) //数据读取时被信号中断 继续读取
        {
            continue;
        }
        else if(readn > 0)//读取到了数据
        {
            printf("接收到数据(来自:%d)：%s\n", fd_, buf);
            send(fd_, buf, strlen(buf), 0);
        }
    }
}

void Channel::setCallback(std::function<void()> fn)//设置回调函数
{
    readcallback_ = fn;
}