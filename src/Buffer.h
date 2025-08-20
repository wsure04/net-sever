#pragma once
#include<string>
#include<iostream>

class Buffer
{
    private:
        std::string buf_; //用于存放数据 不要理解为字符串
    public:
        Buffer();
        ~Buffer();

        void append(const char* data, size_t size);//把数据追加到buf中
        size_t size();//返回buf_的大小
        const char* data();//返回buf_首地址
        void clear();//清空buf_
        void erase(size_t pos, size_t n);
};