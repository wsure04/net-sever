#include"Epoll.h"
/*class Epoll
{
    private:
        static const int MaxEvents = 1024; 
        int epollfd_ = -1;//epoll句柄 在构造函数中创建
        epoll_event events_[MaxEvents];
    public:
        Epoll();//在构造函数中创建 epollfd
        ~Epoll();//在析构函数中 关闭epollfd

        void addfd(int fd, uint32_t op);//把 fd和监听的事件 添加到红黑树上
        std::vector<epoll_event> loop(int timeout = -1);//运行epoll_wait 发生的事件用vector返回
};*/


Epoll::Epoll()
{
    //创建epoll句柄
    if((epollfd_ = epoll_create(1)) == -1)
    {
        printf("epoll_create() failed(%d)\n", errno);
        exit(-1);
    }
}//在构造函数中创建 epollfd
Epoll::~Epoll()
{
    ::close(epollfd_);
}//在析构函数中 关闭epollfd

void Epoll::addfd(int fd, uint32_t op)
{
    epoll_event ev;
    ev.events = op;
    ev.data.fd = fd;
    if(epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1)
    {
        printf("epoll_ctl() failed(%d)\n", errno);
        exit(-1);
    }
}//把 fd和监听的事件 添加到红黑树上
std::vector<epoll_event> Epoll::loop(int timeout)
{
    std::vector<epoll_event> evs;//存放epoll_wait返回的事件
    bzero(events_, sizeof(events_));
    int infd = epoll_wait(epollfd_, events_, MaxEvents, timeout);
    if(infd < 0)//事件失败
    {
        perror("epoll_wait failed");
        exit(-1);
    }

    if(infd == 0)//超时 轮询等待连接
    {
        printf("epoll_wait timeout\n");
        return evs;//返回空的数组对象
    }

    //成功检测到事件
    for(int i = 0; i < infd; i++)
    {
        evs.push_back(events_[i]);
    }
    return evs;
}//运行epoll_wait 发生的事件用vector返回