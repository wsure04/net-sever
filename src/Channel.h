#pragma once
#include<sys/socket.h>
#include<sys/epoll.h>
#include"Epoll.h"

class Epoll;

class Channel//将channel的地址作为epoll携带的数据
{
    private:
        int fd_ = -1; //一对一
        Epoll *ep_ = nullptr;//channel与epoll多对一 一个channel对应一个epoll
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
