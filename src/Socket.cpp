#include"Socket.h"

/*
class Socket
{
    private:
        const int fd;//Socket持有的fd
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
        void accept(InetAddress& client_addr);        
};
*/


int createNonblocking()
{
     int listenfd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
    if(listenfd < 0)
    {
        //perror("socket failed");
        printf("%s:%s:%d listen socket create error:%d\n", __FILE__, __FUNCTION__, __LINE__, errno);
        exit(-1);
    }
    return listenfd;
}//创建非阻塞的socket

    Socket::Socket(int fd):fd_(fd){}
    Socket::~Socket()
    {
        ::close(fd_);
    }

    int Socket::fd() const
    {
        return fd_;
    }//返回fd成员
    void  Socket::setReuseAddr(bool on)
    {
        int opt = on ? 1 : 0;
        ::setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    }//设置SO_REUSEADDR true-打开
    void  Socket::setReusePort(bool on)
    {
        int opt = on ? 1 : 0;
        ::setsockopt(fd_, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    }//设置SO_REUSEPORT true-打开
    void  Socket::setTcpNodelay(bool on)
    {
        int opt = on ? 1 : 0;
        ::setsockopt(fd_, SOL_SOCKET, TCP_NODELAY, &opt, sizeof(opt));
    }//设置TCP_NODELAY true-打开
    void  Socket::setKeepAlive(bool on)
    {
        int opt = on ? 1 : 0;
        ::setsockopt(fd_, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(opt));
    }//设置SO_KEEPALIVE true-打开

    void  Socket::bind(const InetAddress& serv_addr)
    {
        if(::bind(fd_, serv_addr.addr(), sizeof(sockaddr)) < 0)
        {
            perror("bind() failed");
            close(fd_);
            exit(-1);
        }
    }
    void  Socket::listen(int n)
    {
        if(::listen(fd_, n) < 0)
        {
            perror("listen() failed");
            close(fd_);
            exit(-1);
        }
    }
    int  Socket::accept(InetAddress& client_addr)
    {
        sockaddr_in peer_addr;
        bzero(&peer_addr, sizeof(peer_addr));
        socklen_t client_len = sizeof(peer_addr);
        int clientfd = accept4(fd_, (sockaddr*)&peer_addr, &client_len, SOCK_NONBLOCK);//自动设置非阻塞

        client_addr.setAddr(peer_addr);

        return clientfd;
    }        