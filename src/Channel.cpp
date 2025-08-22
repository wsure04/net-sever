#include"Channel.h"
#include"Connection.h"

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
    loop_->updateChannel(this);
}

void Channel::disableReading()
{
    events_ &= ~EPOLLIN;
    loop_->updateChannel(this);
} //取消读事件
void Channel::enableWriting()
{
    events_ |= EPOLLOUT;
    loop_->updateChannel(this);
} //让epoll_wait监听fd_的写事件
void Channel::disableWriting()
{
    events_ &= ~EPOLLOUT;
    loop_->updateChannel(this);
}//取消写事件

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
        closecallback_();
    }
    else if(revents_ & (EPOLLIN|EPOLLPRI))
    {
        readcallback_();
    }
    else if(revents_ & EPOLLOUT)
    {
        writecallback_();
    }
    else//其他是为错误
    {
        errorcallback_();
    }
}//事件处理函数 epoll_wait()返回的时候，执行它

void Channel::setReadCallback(std::function<void()> fn)//设置回调函数
{
    readcallback_ = fn;
}

void Channel::setCloseCallback(std::function<void()> fn)
{
    closecallback_ = fn;
}
void Channel::setErrorCallback(std::function<void()> fn)
{
    errorcallback_ = fn;   
}


void Channel::setWriteCallback(std::function<void()> fn)
{
    writecallback_ = fn;
}