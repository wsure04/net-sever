#pragma once
#include<sys/socket.h>
#include<sys/epoll.h>
#include<functional>
#include"EventLoop.h"
#include"InetAddress.h"
#include"Socket.h"

class EventLoop;

class Channel//将channel的地址作为epoll携带的数据
{
    private:
        int fd_ = -1; //一对一
        //Epoll *ep_ = nullptr;//channel与epoll多对一 一个channel对应一个epoll
        EventLoop *loop_ = nullptr;
        bool inepoll_ = false;//channel是否已经添加到红黑树上 如果已经添加 用ADD 否则用MOD
        uint32_t events_ = 0;//fd_需要监视的事件
        uint32_t revents_ = 0;//fd已经发生的事件
        //bool islisten_ = false;//如果是listenfd取值为true
        std::function<void()> readcallback_;//fd_读事件的回调函数，如果是acceptchannel, 将回调newconnection
        std::function<void()> closecallback_;//关闭fd_的回调函数， 将回调Connection::closecallback()
        std::function<void()> errorcallback_;//fd_发生了错误的回调函数，将回调Connection::errorcallback()
    public:
        Channel(EventLoop *loop, int fd);
        ~Channel();

        int fd();
        void useET();//设置采用边缘触发
        void enableReading(); //让epoll_wait监听fd_的读事件
        void setInepoll(); //设置inepoll未true
        void setRevents(uint32_t ev);//设置revents成员函数
        bool inpoll();
        uint32_t events();
        uint32_t revents();

        void handleEvent();//事件处理函数 epoll_wait()返回的时候，执行它

        //void onMessage();//处理对端发来的消息

        void setReadCallback(std::function<void()> fn);//设置回调函数
        void setCloseCallback(std::function<void()> fn);
        void setErrorCallback(std::function<void()> fn);
};
