#include<iostream>
#include<netinet/tcp.h>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<sys/epoll.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<vector>
#include"InetAddress.h"
#include"Socket.h"
#include"Epoll.h"
#include"EventLoop.h"
using std::cout;using std::endl;

void setnoblock(int fd)
{
    if(fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK) == -1)
    {
        perror("fcntl");
    }
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "请输入ip地址 端口号\n";
        return -1;
    }

    Socket serv_sock(createNonblocking());//封装好的套接字
    InetAddress serv_addr(argv[1], atoi(argv[2]));//封装好的地址结构
    //设置sock属性
    const bool on = true;
    serv_sock.setReuseAddr(on);
    serv_sock.setReusePort(on);
    serv_sock.setTcpNodelay(on);
    serv_sock.setKeepAlive(on);
    //绑定地址结构
    serv_sock.bind(serv_addr);
    //设置监听上限
    serv_sock.listen();

    //创建epoll句柄
    EventLoop loop;

    Channel* serv_channel = new Channel(&loop, serv_sock.fd());
    serv_channel->setCallback(std::bind(&Channel::newConnection, serv_channel, &serv_sock));
    serv_channel->enableReading();

    loop.run();

    return 0;
}