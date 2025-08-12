#include<iostream>
#include<sys/socket.h>
#include<errno.h>
#include<string.h>
#include<time.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<netinet/in.h>
#include<netdb.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
   if(argc != 3)
   {
    printf("请输入ip地址 端口号\n");
    return -1;
   } 

   int sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if(sockfd < 0)
   {
    perror("socket");
    return -1;
   }
   struct sockaddr_in serv_addr;
   bzero(&serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(atoi(argv[2]));
   if((inet_pton(AF_INET,  argv[1], &serv_addr.sin_addr)) < 0)
   {
        perror("inet_pton");
        return -1;
   }
   
   if(connect(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
        perror("connect()");
        return -1;
   }
   printf("已连接服务器\n");
   char buf[BUFSIZ];
   for(;;)
   {
        bzero(buf, sizeof(buf));

        printf("请输入发送数据：");
        scanf("%s", buf);

        if(send(sockfd, buf, strlen(buf), 0) < 0)
        {
            perror("send() failed");
            close(sockfd);
            return -1;
        }

        bzero(buf, sizeof(buf));


        if(recv(sockfd, buf, sizeof(buf), 0) < 0)
        {
            perror("recv() failed");
            close(sockfd);
            return -1;
        }

        printf("接收到数据：%s\n", buf);

   }

}