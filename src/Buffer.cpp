#include"Buffer.h"

/*
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
};
*/
Buffer::Buffer()
{

}
Buffer::~Buffer()
{

}
void Buffer::append(const char* data, size_t size)//把数据追加到buf中
{

    buf_.append(data, size);
}
size_t Buffer::size()//返回buf_的大小
{
    return buf_.size();
}
const char* Buffer::data()//返回buf_首地址
{
    return buf_.data();
}
void Buffer::clear()//清空buf_
{
    buf_.clear();
}

void Buffer::erase(size_t pos, size_t n)
{
    buf_.erase(pos, n);
}