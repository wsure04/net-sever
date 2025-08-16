#pragma once
#include"Epoll.h"
class Channel;
class Epoll;
//事件循环类
class EventLoop
{
    private:
        Epoll *ep_;//一对一
    public:
    EventLoop();//在构造函数中 创建epoll对象
    ~EventLoop();//在析构函数中销毁ep对象

    void run(); //运行事件循环

  

    void updateChannel(Channel* ch);//将Channel添加或更新到红黑树上 Channel中也有fd
};