#include<signal.h>
#include"EchoServer.h"

using std::cout;using std::endl;

EchoServer *echo_server;

void Stop(int sig)
{
    printf("sig=%d\n", sig);
    echo_server->Stop();
    printf("echo_server已停止.\n");
    delete echo_server;
    printf("delete echoserver.\n");
    exit(0);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        cout << "请输入ip地址 端口号\n";
        return -1;
    }

    signal(SIGTERM, Stop);
    signal(SIGINT, Stop);

    echo_server = new EchoServer(argv[1], atoi(argv[2]));

    echo_server->Start();

    return 0;
}