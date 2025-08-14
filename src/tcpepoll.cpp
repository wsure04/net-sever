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
    Epoll ep;
    Channel* serv_channel = new Channel(&ep, serv_sock.fd(), true);
    serv_channel->enableReading();
    while(true)
    {

        std::vector<Channel*> channels = ep.loop();//等待事件
        
        for(auto &ch : channels)
        {
            //处理读写事件
            ch->handleEvent(&serv_sock);
        }
        
    }
    return 0;
}