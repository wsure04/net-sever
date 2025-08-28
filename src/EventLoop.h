#pragma once
#include"Epoll.h"
#include<functional>
#include<sys/syscall.h>
#include<unistd.h>
class Channel;
class Epoll;
//事件循环类
class EventLoop
{
    private:
        Epoll *ep_;//一对一
        std::function<void(EventLoop*)> epolltimeoutcallback_;  //epoll_wait超时的回调函数
    public:
    EventLoop();//在构造函数中 创建epoll对象
    ~EventLoop();//在析构函数中销毁ep对象

    void run(); //运行事件循环

    void updateChannel(Channel* ch);//将Channel添加或更新到红黑树上 Channel中也有fd
    void setEpollwaitTimeout(std::function<void(EventLoop*)> fn);

    void removeChannel(Channel* ch); //从红黑树上删除channel
};