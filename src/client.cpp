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
     for(int i = 0;i < 1; i++)
     {
        bzero(buf, sizeof(buf));

        sprintf(buf,"这是第%d条报文。", i);
        //scanf("%s", buf);

        char tmpbuf[1024];  //临时的buf 报文头部+报文内容
        bzero(tmpbuf, sizeof(tmpbuf));
        int len = strlen(buf);//计算报文大小
        memcpy(tmpbuf, &len, 4);
        memcpy(tmpbuf+4, buf, len);
        send(sockfd, tmpbuf, len+4, 0);
     }
     for(int i = 0;i < 1; i++)
     {
          int len;
          recv(sockfd, &len, 4, 0);

          bzero(buf, sizeof(buf));

          recv(sockfd, buf, len, 0);
          
          printf("接收到数据：%s\n", buf);

     }
     sleep(100);
}