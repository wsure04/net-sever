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
    ep.addfd(serv_sock.fd(), EPOLLIN);
    std::vector<epoll_event> evs;//接收有事件的对象
    while(true)
    {
        evs = ep.loop();//等待事件
        
        for(auto &ev : evs)
        {
            //处理读写事件
            if(ev.events & EPOLLRDHUP)
                {
                    printf("客户端(%d)已关闭\n", ev.data.fd);
                    close(ev.data.fd);//关闭客户端
                }
            else if(ev.events & EPOLLIN|EPOLLPRI)
            {
                if(ev.data.fd == serv_sock.fd())//处理监听事件 监听事件也是读事件
                {
                    InetAddress client_addr;
                    //接收客户端socket
                    Socket *client_sock = new Socket(serv_sock.accept(client_addr));

                    printf("客户端(fd:%d, ip:%s, port:%d)连接\n", client_sock->fd(), client_addr.ip(), client_addr.port());

                    ep.addfd(client_sock->fd(), EPOLLIN | EPOLLET);
                }
                else
                {
                    char buf[BUFSIZ];
                    //注意 使用的是非阻塞io
                    int readn = 0;
                    while(true)
                    {
                        bzero(buf, sizeof(buf));
                        readn = recv(ev.data.fd, buf, sizeof(buf), 0);
                        if(readn == 0)
                        {    
                            printf("客户端(%d)已关闭\n", ev.data.fd);
                            close(ev.data.fd);//关闭客户端
                        }
                        else if(readn == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))//数据读取完毕 非阻塞立即返回跳出
                        {
                            break;
                        }
                        else if(readn == -1 && errno == EINTR) //数据读取时被信号中断 继续读取
                        {
                            continue;
                        }
                        else if(readn > 0)//读取到了数据
                        {
                            printf("接收到数据(来自:%d)：%s\n", ev.data.fd, buf);
                            send(ev.data.fd, buf, strlen(buf), 0);
                        }
                    }
                }
            }
            else if(ev.events & EPOLLOUT)
            {

            }
            else//其他是为错误
            {
                printf("client(%d) error\n", ev.data.fd);
                close(ev.data.fd);
            }
        }
        
    }
    return 0;
}