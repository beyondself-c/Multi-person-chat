#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<poll.h>
#include "_public.h"

#define USER_FDS 1024/*最大用户数量*/
#define BUFFER_SIZE 64/*读缓冲区的大小*/
#define FD_LIMIT 65535/*文件描述符数量限制*/

// 初始化服务端的socket，port为通信端口
// 返回值：成功返回初始化的socket，失败返回-1。
int initserver(int port);

CLogFile LogFile;

struct client_data
{
  sockaddr_in address;
  char write_buf[1024];
  char buf[BUFFER_SIZE];  // BUFFER_SIZE = 64
};

int setnonblocking(int fd)
{
  int old_option=fcntl(fd,F_GETFL);     // 获取文件描述符旧的状态标志
  int new_option=old_option|O_NONBLOCK; // 设置非阻塞标志
  fcntl(fd,F_SETFL,new_option);         // 
  return old_option;                    // 返回文件描述符旧的状态标志，以便日后恢复该状态标志
}
 

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    printf("Using:./pollserver port\n\n");return -1;
  }

  if( LogFile.Open("logfile","a+") == false) printf("open logfile failed\n");

  int listenfd = initserver(atoi(argv[1]));
  if(listenfd < 0 ) { LogFile.Write("initserver failed\n"); return -1; }

  client_data*users=new client_data[FD_LIMIT]; 

  int maxfd = 0;
  struct pollfd fds[USER_FDS];
  for(int ii = 0; ii<USER_FDS;ii++){ fds[ii].fd = -1;fds[ii].events = 0;}

  fds[listenfd].fd = listenfd;
  fds[listenfd].events = POLLIN|POLLOUT;
  maxfd = listenfd;

  char name[BUFFER_SIZE];

  while(1)
  {
    int infds = poll(fds,maxfd + 1,-1);
    
    if(infds < 0)
    {
      LogFile.Write("poll() failed.\n"); perror("poll():"); break;
    }
  
    if(infds == 0)
    {
      LogFile.Write("poll() timeout.\n"); continue;
    }

    for(int eventfd = 0; eventfd <= maxfd; eventfd++)
    {
      if(fds[eventfd].fd < 0) continue;

      if( (eventfd == listenfd) &&(fds[eventfd].revents&POLLIN))
      {
        // 如果发生事件的是listensock，表示有新的客户端连上来。
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int clientfd = accept(listenfd,(struct sockaddr*)&client,&len);
        if (clientfd < 0)
        {
          LogFile.Write("accept() failed.\n"); continue;
        }

        if(maxfd >= USER_FDS)
        {
          const char *buffer ="too many pepole,will close connect\n";
          LogFile.Write("%s\n","too many pepole,close this new users");
          write(clientfd,buffer,strlen(buffer));
          usleep(10000); 
          close(clientfd);
          continue;
        }
 	fds[clientfd].fd=clientfd;
        setnonblocking(clientfd);
        fds[clientfd].events=POLLIN; 
        fds[clientfd].revents=0; 
        if (maxfd < clientfd) maxfd = clientfd;
        LogFile.Write ("comes a new user %d,now have %d users\n\n",clientfd,clientfd -listenfd );
         
      }

      else if(fds[eventfd].revents&POLLIN)
      {
        int clientfd = fds[eventfd].fd;
        memset(users[clientfd].buf,0,BUFFER_SIZE);
        ssize_t isize = read(clientfd,users[clientfd].buf,sizeof(users[clientfd].buf));
        if(isize <= 0)
        {
          LogFile.Write("client(eventfd=%d) disconnected.\n",eventfd);
          LogFile.Write("now have %d users\n",clientfd - listenfd - 1);
          close(eventfd);  // 关闭客户端的socket。

          fds[eventfd].fd=-1;

          // 重新计算maxfd的值，注意，只有当eventfd==maxfd时才需要计算。
          if (eventfd == maxfd)
          {
            for (int ii=maxfd;ii>0;ii--)
            {
              if ( fds[ii].fd != -1)
              {
                maxfd = ii; break;
              }
            }

            //LogFile.Write("maxfd=%d\n",maxfd);
          }

          continue;
        }

        else
        {
          for(int ii =maxfd; ii > listenfd; ii--)
          {  
	    memset(name,0,BUFFER_SIZE);
            memset(users[ii].write_buf,0,sizeof(users[ii].write_buf));
            sprintf(name,"from user %d \n",clientfd);

            if(fds[ii].fd == clientfd)  continue;
            fds[fds[ii].fd].events |=~POLLIN;
            fds[fds[ii].fd].events |=POLLOUT;
            
            strncat(users[ii].write_buf,name,sizeof(name));
            strncat(users[ii].write_buf,users[clientfd].buf,sizeof(users[clientfd].buf));
          }     
        LogFile.Write("get %d bytes of client data [%s] from %d\n",isize,users[clientfd].buf,clientfd);
        }
      }

      else if(fds[eventfd].revents&POLLOUT)
      {
        int clientfd = fds[eventfd].fd;
        if(!users[eventfd].write_buf) continue;
        infds = write(clientfd,users[eventfd].write_buf,strlen(users[eventfd].write_buf));
        memset(users[eventfd].write_buf,0,sizeof(users[eventfd].write_buf));
        fds[eventfd].events |=~POLLOUT;
        fds[eventfd].events |=POLLIN;

      }
    }

  }

  delete[]users;
  close(listenfd);
  return 0;

}


int initserver(int port)
{
  int listenfd = socket(AF_INET,SOCK_STREAM,0);  // 创建服务端的socket

// Linux如下
  int opt = 1; unsigned int len = sizeof(opt);
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);
  setsockopt(listenfd,SOL_SOCKET,SO_KEEPALIVE,&opt,len);
 
  // 把服务端用于通信的地址和端口绑定到socket上
  struct sockaddr_in servaddr;    // 服务端地址信息的数据结构
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本主机的任意ip地址
  servaddr.sin_port = htons(port);  // 绑定通信端口
  if (bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) != 0 )
  { perror("bind"); close(listenfd); return -1; }
 
  // 把socket设置为监听模式
  if (listen(listenfd,5) != 0 ) { perror("listen"); close(listenfd); return -1; }
 
  return listenfd;
}
