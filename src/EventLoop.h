#pragma once
#include"Epoll.h"

//事件循环类
class EventLoop
{
    private:
        Epoll *ep_;
    public:
    EventLoop();//在构造函数中 创建epoll对象
    ~EventLoop();//在析构函数中销毁ep对象

    void run(); //运行事件循环

    Epoll* ep();

};