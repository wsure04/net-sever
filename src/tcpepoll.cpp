
#include<iostream>
/*
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
*/
#include"TcpServer.h"
using std::cout;using std::endl;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "请输入ip地址 端口号\n";
        return -1;
    }

    TcpServer tcp_server(argv[1], atoi(argv[2]));

    tcp_server.start();

    return 0;
}