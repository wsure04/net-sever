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
    while(true)
    {
        std::vector<Channel*> channels = ep_->loop();//等待事件
        
        for(auto &ch : channels)
        {
            //处理读写事件
            ch->handleEvent();
        }
    }
    
} //运行事件循环


void EventLoop::updateChannel(Channel* ch)//将Channel添加或更新到红黑树上 Channel中也有fd 对应addfd
{
    ep_->updateChannel(ch);
}