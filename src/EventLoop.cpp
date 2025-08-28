#include"EventLoop.h"

/*
//事件循环类
class EventLoop
{
    private:
        Epoll *ep_;
    public:
    EventLoop();//在构造函数中 创建epoll对象
    ~EventLoop();//在析构函数中销毁ep对象

    void run(); //运行事件循环
};
*/
EventLoop::EventLoop():ep_(new Epoll){}//在构造函数中 创建epoll对象

EventLoop::~EventLoop()
{
    delete ep_;
}//在析构函数中销毁ep对象

void EventLoop::run()
{
    //printf("EventLoop::run() thread is %d.\n", syscall(SYS_gettid));
    while(true)
    {
        std::vector<Channel*> channels = ep_->loop(10*1000);//等待事件
        
        //如果channels为空，表示超时，回调TcpServer::epolltimeout()
        if(channels.size() == 0)
        {
            epolltimeoutcallback_(this);
        }
        else
        {
            for(auto &ch : channels)
            {
                //处理读写事件
                ch->handleEvent();
            }
        }
    }
    
} //运行事件循环


void EventLoop::updateChannel(Channel* ch)//将Channel添加或更新到红黑树上 Channel中也有fd 对应addfd
{
    ep_->updateChannel(ch);
}

void EventLoop::removeChannel(Channel* ch)
{
    ep_->removeChannel(ch);
} //从红黑树上删除channel

void EventLoop::setEpollwaitTimeout(std::function<void(EventLoop*)> fn)
{
    epolltimeoutcallback_ = fn;
}