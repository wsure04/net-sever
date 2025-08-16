#pragma once

#include<arpa/inet.h>
#include<netinet/in.h>
#include<string>

//socket地址协议类
class InetAddress
{
    private:
        struct sockaddr_in addr_;
    public:
        InetAddress();
        InetAddress(const std::string &ip, const uint16_t port);//监听套接字的构造函数
        InetAddress(const sockaddr_in addr);//客户端套接字的构造函数 接收accept的传出参数
        ~InetAddress();

        const char *ip() const;//返回字符串格式的ip
        uint16_t port() const;//获取端口号
        const sockaddr *addr() const;//返回转换的地址

        void setAddr(sockaddr_in client_addr);

};