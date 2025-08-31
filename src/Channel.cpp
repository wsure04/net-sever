#include"Channel.h"
#include"Connection.h"

Channel::Channel(const std::unique_ptr<EventLoop>& loop, int fd):loop_(loop), fd_(fd){}//初始化成员变量
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
    events_ |= EPOLLIN;
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

void Channel::disableAll()
{
    events_ = 0;
    loop_->updateChannel(this);
}//取消全部事件
void Channel::remove()
{
    disableAll();
    loop_->removeChannel(this);
} //从事件循环中删除Channel

void Channel::setInepoll(bool inepoll) //设置inepoll未true
{
    inepoll_ = inepoll;
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
        //remove();    //从事件循环中删除Channel
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
        //remove();
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