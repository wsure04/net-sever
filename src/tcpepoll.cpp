
#include"EchoServer.h"

using std::cout;using std::endl;

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "请输入ip地址 端口号\n";
        return -1;
}

    EchoServer echo_server(argv[1], atoi(argv[2]));

    echo_server.Start();

    return 0;
}