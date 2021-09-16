#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>   // assert
#include<stdio.h>
#include<unistd.h>   // pipe
#include<string.h>
#include<stdlib.h>
#include<poll.h>
#include <netdb.h>   //gethostbyname
#include<fcntl.h>    // splice
#define BUFFER_SIZE 64


int main(int argc, char *argv[])
{
  
  if(argc != 3)
  {
    printf("Using: ./pollclient ip port\n"); return -1;    
  }

  int sockfd;
  if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
 
  // 第2步：向服务器发起连接请求。
  struct hostent* h;
  if ( (h = gethostbyname(argv[1])) == 0 )   // 指定服务端的ip地址。
  { printf("gethostbyname failed.\n"); close(sockfd); return -1; }
  struct sockaddr_in servaddr;
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(atoi(argv[2])); // 指定服务端的通信端口。
  memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
  if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  // 向服务端发起连接清求。
  { perror("connect"); close(sockfd); return -1; }

  struct pollfd fds[2];
  fds[0].fd = 0;
  fds[0].events = POLLIN;
  fds[0].revents = 0;
  fds[1].fd = sockfd;
  fds[1].events = POLLIN|POLLRDHUP;
  fds[1].revents = 0;
 
  int iret = 0;
  char buffer[1024];
  int pipefd[2];
  iret=pipe(pipefd);
  if( iret == -1){ perror("pipe"); return -1;} 

  while(1)
  {
    iret = poll(fds,2,-1);

    if(iret < 0)
    {
      printf("poll failed\n");
    }
  
    if(iret == 0)
    { 
      printf("poll() timeout\n");
    }

    if(fds[1].revents&POLLRDHUP)
    {
      printf("Server close connect\n");
      break;
    }
    else if(fds[1].revents&POLLIN)
    {
      memset(buffer,0,sizeof(buffer));
      read(sockfd,buffer,sizeof(buffer));
      printf("%s\n",buffer);
    }

    if(fds[0].revents&POLLIN)
    {
      iret=splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
      iret=splice(pipefd[0],NULL,sockfd,NULL,32768,SPLICE_F_MORE|SPLICE_F_MOVE);
    }
  }
  
  close(sockfd);
  return 0;

}

// memset(buffer,0,sizeof(buffer));
// scanf("%s",buffer);
// write(sockfd,buffer,strlen(buffer)); 
