#pragma once
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/tcp.h>
#include<string.h>
#include<errno.h>
#include<unistd.h>
#include"InetAddress.h"

int createNonblocking();//创建非阻塞的socket

//socket类
class Socket
{
    private:
        const int fd_;//Socket持有的fd
    public:
        Socket(int fd);
        ~Socket();

        int fd() const;//返回fd成员
        void setReuseAddr(bool on);//设置SO_REUSEADDR true-打开
        void setReusePort(bool on);//设置SO_REUSEPORT true-打开
        void setTcpNodelay(bool on);//设置TCP_NODELAY true-打开
        void setKeepAlive(bool on);//设置SO_KEEPALIVE true-打开

        void bind(const InetAddress& serv_addr);
        void listen(int n = 128);
        int accept(InetAddress& client_addr);        
};