#include"InetAddress.h"
/*
class InetAddress
{
    private:
        struct sockaddr_in addr_;
    public:
        InetAddress(const std::string &ip, const uint16_t port);//监听套接字的构造函数
        InetAddress(const sockaddr_in addr):addr_(addr){};//客户端套接字的构造函数 接收accept的传出参数
        ~InetAddress();

        const char *ip() const;//返回字符串格式的ip
        uint16_t port() const;//获取端口号
        const sockaddr *addr() const;//返回转换的地址 可以用于传入bind
};
*/

InetAddress::InetAddress(){};

InetAddress::InetAddress(const std::string &ip, const uint16_t port)
{
    addr_.sin_family = AF_INET;
    addr_.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}
InetAddress::InetAddress(const sockaddr_in addr):addr_(addr){}//客户端套接字的构造函数 接收accept的传出参数

InetAddress::~InetAddress(){}

const char *InetAddress::ip() const
{
    char str[INET_ADDRSTRLEN];
    return inet_ntop(AF_INET, &addr_.sin_addr, str, INET_ADDRSTRLEN);
}//返回字符串格式的ip
uint16_t InetAddress::port() const
{
   return ntohs(addr_.sin_port);
}//获取端口号
const sockaddr *InetAddress::addr() const
{
    return (sockaddr*)&addr_;
}//返回转换的地址 可以用于传入bind

void InetAddress::setAddr(sockaddr_in client_addr)
{
    addr_ = client_addr;
}