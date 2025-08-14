#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<strings.h>
#include<sys/epoll.h>
#include<vector>
#include<unistd.h>
#include"Channel.h"

class Channel;

//Epoll类
class Epoll
{
    private:
        static const int MaxEvents = 1024; 
        int epollfd_ = -1;//epoll句柄 在构造函数中创建
        epoll_event events_[MaxEvents];
    public:
        Epoll();//在构造函数中创建 epollfd
        ~Epoll();//在析构函数中 关闭epollfd

        void addfd(int fd, uint32_t op);//把 fd和监听的事件 添加到红黑树上

        //std::vector<epoll_event> loop(int timeout = -1);//运行epoll_wait 发生的事件用vector返回
        std::vector<Channel*> loop(int timeout = -1);//运行epoll_wait 发生的事件用vector返回

        void updateChannel(Channel* ch);//将Channel添加或更新到红黑树上 Channel中也有fd
};