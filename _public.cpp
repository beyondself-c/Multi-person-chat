/*
 *程序名: _public.h，公共功能函数声明的头文件
 *
*/
#include "_public.h"


// 求x的y次幂，函数返回值为x的y次幂。
unsigned long  POW(const unsigned int x,unsigned const int y)
{
   if(y==0)return 1;

   int i=1;
   unsigned long  result=1;
   
   for(i=1;i<=y;i++)
   {
     result=result*x;
   }
  return result;
}



// 用于把数字的字符转换为整数，chr为用字符方式表示的数字，函数的返回值为数字的整数。
// 如果chr不是数字的字符，则返回-1。
int ctoi(const char chr)
{
   if((chr>='0')&&(chr<='9')) return chr-'0';
   return -1;


/*
 switch (chr)
{
  case ('0'): return 0;
  case ('1'): return 1;
  case ('2'): return 2;
  case ('3'): return 3;
  case ('4'): return 4;
  case ('5'): return 5;
  case ('6'): return 6;
  case ('7'): return 7;
  case ('8'): return 8;
  case ('9'): return 9;
  default:return -1;
 }
*/
}


//四舍五入

double ROUND(const double x)
{
    if(x==0) return 0;

    long ii;
    if(x>0)
  {
    ii=x+0.5; return ii;
  }
    else
  {
     ii=x-0.5; return ii;
   }
}

//向上取整
double CEIL(const double x)
{
  long ii;
  if(x>0) {ii=x+1;return ii;}
  else {ii=x;return ii;}
}

//向下取整
double FLOOR(const double x)
{
  long ii;
  if(x>=0){ii=x;return ii;}
  else{ii=x-1;return ii;}
}

//把双精度val分解成整数部分和小数部分，整数部分存放在ip所指的变量中，返回小数部分。
double MODF(double val,double *ip)
{
   long ii;
   ii=val;
   *ip=ii;
  return val-ii;
}


CFile::CFile() //构造函数
{
    m_fp=0;
    m_bEnBuffer=true;
}

CFile:: CFile( bool bEnBuffer)
{
    m_fp=0;
    m_bEnBuffer=bEnBuffer;

}

CFile::~CFile()                //析构函数
{
     Close();
}


void CFile::EnBuffer(bool bEnBuffer) // 启、禁用缓冲区
{
   m_bEnBuffer=bEnBuffer;

}

// 打开文件，参数与fopen相同，打开成功true，失败返回false
bool CFile::Open(const char *filename,const char *filemode)
{
   Close();

   if( (m_fp=fopen(filename,filemode))==0 )  return false;
      return true;
}

 
//调用fprintf向文件写入数据
void CFile::Fprintf(const char *fmt,...)
{
   if(m_fp==0) return;

   va_list arg;
   va_start(arg,fmt);
   vfprintf(m_fp,fmt, arg);
   va_end(arg);

   if ( m_bEnBuffer == false ) fflush(m_fp);
}

// 调用fgets从文件中读取一行
bool CFile::Fget(char *strBuffer,const int readsize)
{
    if(m_fp==0) return false;
    memset(strBuffer,0,sizeof(strBuffer));

   if( fgets(strBuffer,readsize,m_fp)==0) return false;

   return true;
}
 
//关闭文件指针
void CFile::Close()
{
  if(m_fp!=0) fclose(m_fp);
  m_fp=0;

}


void CRang::Rang(const int minvalue,const int maxvalue,const int totalcount,\
                  const bool brep)
{
//随机数不可以重复
while(brep==false)
 {

   int result[maxvalue];
   memset(result,0,sizeof(result));
   srand(time(0));
   int  ii=0;
   int pos=0;
  for( ii=0;pos<maxvalue;ii++)
  {
   int irand=  rand()%maxvalue+minvalue;
   int jj=0;
   for( jj=0;jj<maxvalue;jj++)
   {

      if(irand==result[jj])  break;

   }
   if(jj<maxvalue) continue;

     result[pos]=irand;
     pos++;
     printf("=%d=\n",irand);
     m_val.push_back(irand);
 }
  break ;
}

//随机数可以重复
while(brep==true)
{
   srand(time(0));
   for(int ii=0;ii<maxvalue;ii++)
   {
      int irand=rand()%maxvalue+minvalue;
      m_val.push_back(irand);
   }
  break;
}
//把存储在vector<int> m_val的数输出
  printf("m_val.size=%d\n",m_val.size());

   sort(m_val.begin(),m_val.end());

   for(int ii=0;ii<m_val.size();ii++)
   {
     printf("=%d=\n",m_val[ii]);
   }

}



//C++的方法封装socket客户端
CTcpClient::CTcpClient()
{
  m_sockfd=-1;
  memset(m_ip,0,sizeof(m_ip));
  m_port=0;
  m_btimeout=false;
}
 
// 向服务器发起连接，ip-服务端ip，port通信端口
bool CTcpClient::ConnectToServer(const char *ip,const int port)
{
  if(m_sockfd!=-1) { Close(); m_sockfd=-1;  };

  strcpy(m_ip,ip);
  m_port=port;
 
  struct hostent* h;  // ip地址信息的数据结构
  struct sockaddr_in servaddr;

  // 创建客户端的socket
  if ( (m_sockfd = socket(AF_INET,SOCK_STREAM,0)) <0) return false;  
 
  if ( (h=gethostbyname(m_ip))==0 )
  { close(m_sockfd); m_sockfd=-1; return false; }
 
  // 把服务器的地址和端口转换为数据结构
  
  memset(&servaddr,0,sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(port);
  memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
 
  // 向服务器发起连接请求
  if (connect(m_sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
  { close(m_sockfd); m_sockfd=-1; return false; }
 
  return true;

}

bool CTcpClient::Read(char *buffer,const int itimeout)
{
  if (m_sockfd == -1) return false;

  if (itimeout>0)
  {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(m_sockfd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout; timeout.tv_usec = 0;

    m_btimeout = false;

    int i;
    if ( (i = select(m_sockfd+1,&tmpfd,0,0,&timeout)) <= 0 )
    {
      if (i==0) m_btimeout = true;
      return false;
    }
  }

  m_buflen = 0;
  return (TcpRead(m_sockfd,buffer,&m_buflen));
}

bool CTcpClient::Write(const char *buffer,const int ibuflen)
{
  if (m_sockfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_sockfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;

  m_btimeout = false;

  int i;
  if ( (i=select(m_sockfd+1,0,&tmpfd,0,&timeout)) <= 0 )
  {
    if (i==0) m_btimeout = true;
    return false;
  }

  int ilen=ibuflen;

  if (ibuflen==0) ilen=strlen(buffer);

  return(TcpWrite(m_sockfd,buffer,ilen));

}

void CTcpClient::Close()
{
  if(m_sockfd>0) close(m_sockfd);
  m_sockfd=-1;
  memset(m_ip,0,sizeof(m_ip));
  m_port=0;
  m_btimeout=false;
}

CTcpClient::~CTcpClient()
{
  Close();  // 析构函数关闭m_sockfd
}

/*
// 向服务端发送报文
int CTcpClient::Send(const void *buf,const int buflen)
{
  return send(m_sockfd,buf,buflen,0);
}
// 接收对端的报文
int CTcpClient::Recv(void *buf,const int buflen)
{
  return recv(m_sockfd,buf,buflen,0);
}
*/

//C++的方法封装socket服务端
CTcpServer::CTcpServer()
{
  m_socklen=1;
  m_listenfd=-1;
  m_connfd=0;
  m_btimeout=false;
}
 
// 初始化服务端的socket，port为通信端口
bool CTcpServer::InitServer(const unsigned int port)  // 初始化服务端
{
  if( m_listenfd >0) { close(m_listenfd); m_listenfd=-1;}

  m_listenfd = socket(AF_INET,SOCK_STREAM,0);  // 创建服务端的socket
 

  // Linux如下
  int opt = 1; unsigned int len = sizeof(opt);
  setsockopt(m_listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,len);


  // 把服务端用于通信的地址和端口绑定到socket上
  memset(&m_servaddr,0,sizeof(m_servaddr));
  m_servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET
  m_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);  // 本主机的任意ip地址
  m_servaddr.sin_port = htons(port);  // 绑定通信端口
  if (bind(m_listenfd,(struct sockaddr *)&m_servaddr,sizeof(m_servaddr)) != 0 )
  { CloseListen();  return false; }
 
  // 把socket设置为监听模式
  if (listen(m_listenfd,5) != 0 ) {CloseListen(); return false; }
 
  m_socklen=sizeof(struct sockaddr_in);

  return true;
}

bool CTcpServer::Accept()  // 等待客户端的连接
{
  if(m_listenfd==-1) return false;
  if ( (m_connfd=accept(m_listenfd,(struct sockaddr* )&m_clientaddr,\
                       (socklen_t*)&m_socklen)) <= 0) return false;
 
  return true;
}

char* CTcpServer::GetIP()
{
  return ( inet_ntoa(m_clientaddr.sin_addr)  );

}

bool CTcpServer::Read(char *buffer,const int itimeout )
{
    if (m_connfd == -1) return false;

  if (itimeout>0)
  {
    fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(m_connfd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout; timeout.tv_usec = 0;

    m_btimeout = false;

    int i;
    if ( (i = select(m_connfd+1,&tmpfd,0,0,&timeout)) <= 0 )
    {
      if (i==0) m_btimeout = true;
      return false;
    }
  }

  m_buflen = 0;
  return(TcpRead(m_connfd,buffer,&m_buflen));
}


bool CTcpServer::Write(const char *buffer,const int ibuflen)
{
  if (m_connfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(m_connfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;

  m_btimeout = false;

  int i;
  if ( (i=select(m_connfd+1,0,&tmpfd,0,&timeout)) <= 0 )
  {
    if (i==0) m_btimeout = true;
    return false;
  }
  int ilen = ibuflen;
  if (ilen==0) ilen=strlen(buffer);

  return(TcpWrite(m_connfd,buffer,ilen));

}

void CTcpServer::CloseListen()
{
  if(m_listenfd>0)
  { close(m_listenfd); m_listenfd=-1;}
}

void CTcpServer::CloseClient()
{
  if(m_connfd>0)
  { close(m_connfd); m_connfd=-1; }

}

CTcpServer::~CTcpServer()
{
  CloseListen(); CloseClient();
}

bool TcpRead(const int sockfd,char *buffer,int *ibuflen,const int itimeout)
{
  if(sockfd==-1) return false;

  if(itimeout>0)
  {
        fd_set tmpfd;

    FD_ZERO(&tmpfd);
    FD_SET(sockfd,&tmpfd);

    struct timeval timeout;
    timeout.tv_sec = itimeout; timeout.tv_usec = 0;

    int i;
    if ( (i = select(sockfd+1,&tmpfd,0,0,&timeout)) <= 0 ) return false;
  }

  (*ibuflen)=0;
  if( (Readn(sockfd,(char*)ibuflen,4)==false)  ) return false;

  (*ibuflen)=ntohl(*ibuflen); // 把网络字节序转换为主机字节序。

  if( (Readn(sockfd,buffer,(*ibuflen) )==false)  ) return false;
  
  return true;
}

bool TcpWrite(const int sockfd,const char *buffer,const int ibuflen)
{
  if (sockfd == -1) return false;

  fd_set tmpfd;

  FD_ZERO(&tmpfd);
  FD_SET(sockfd,&tmpfd);

  struct timeval timeout;
  timeout.tv_sec = 5; timeout.tv_usec = 0;

  if ( select(sockfd+1,0,&tmpfd,0,&timeout) <= 0 ) return false;

  int ilen=0;
  if(ibuflen==0) ilen=strlen(buffer);
  else ilen=ibuflen;

  int ilenn=htonl(ilen);

  char strTBuffer[1024];
  memset(strTBuffer,0,sizeof(strTBuffer));
  memcpy(strTBuffer,&ilenn,4);
  memcpy(strTBuffer+4,buffer,ilen);

  if ( (Writen(sockfd,strTBuffer,ilen+4)) == false) return false;

  return true;

}

bool Readn(const int sockfd,char *buffer,const size_t n)
{
  int nLeft,nread,idx;
 
  nLeft=n;
  idx=0;
  
  while(nLeft>0)
  {
   if( (nread=recv(sockfd,buffer+idx,nLeft,0)) <=0 ) return false;

   idx += nread;
   nLeft -= nread; 
  }
 
  return true;

}

bool Writen(const int sockfd,const char *buffer,const size_t n)
{
  int nLeft,idx,nwritten;

  nLeft=n;
  idx=0;
  
  while(nLeft>0)
  {
    if( (nwritten=send(sockfd,buffer+idx,nLeft,0)) <=0 )  return false;
    
    nLeft -= nwritten;
    idx += nwritten;
  }

  return true;

}

//////////////////////////////////////////////////////////////////////////////////////


/*
  取操作系统的时间，并把整数表示的时间转换为字符串表示的格式。
  stime：用于存放获取到的时间字符串。
  timetvl：时间的偏移量，单位：秒，0是缺省值，表示当前时间，30表示当前时间30秒之后的时>间点，-30表示当前时间30秒之前的时间点。
  fmt：输出时间的格式，缺省是"yyyy-mm-dd hh24:mi:ss"，目前支持以下格式：
  "yyyy-mm-dd hh24:mi:ss"，此格式是缺省格式。
  "yyyymmddhh24miss"
  "yyyy-mm-dd"
  "yyyymmdd"
  "hh24:mi:ss"
  "hh24miss"
  "hh24:mi"
  "hh24mi"
  "hh24"
  "mi"
  注意：
    1）小时的表示方法是hh24，不是hh，这么做的目的是为了保持与数据库的时间表示方法一致；
    2）以上列出了常用的时间格式，如果不能满足你应用开发的需求，请修改源代码增加更多的\ 格式支持；
    3）调用函数的时候，如果fmt与上述格式都匹配，stime的内容将为空。
*/
void LocalTime(char *stime,const char *fmt,const int timetvl)
{
  if (stime==0) return;

  time_t  timer;

  time( &timer ); timer=timer+timetvl;

  timetostr(timer,stime,fmt);
}

///////////////////////////////////////////////////////////////////////////////////////

// 把整数表示的时间转换为字符串表示的时间。
// ltime：整数表示的时间。
// stime：字符串表示的时间。
// fmt：输出字符串时间stime的格式，与LocalTime函数的fmt参数相同，如果fmt的格式不正确，\stime将为空。
void timetostr(const time_t ltime,char *stime,const char *fmt)
{
  if (stime==0) return;

  strcpy(stime,"");

  struct tm sttm = *localtime ( &ltime );

  sttm.tm_year=sttm.tm_year+1900;
  sttm.tm_mon++;

  if (fmt==0)
  {
    snprintf(stime,20,"%04u-%02u-%02u %02u:%02u:%02u",sttm.tm_year,\
                    sttm.tm_mon,sttm.tm_mday,sttm.tm_hour,\
                    sttm.tm_min,sttm.tm_sec);
    return;
  }

  if (strcmp(fmt,"yyyy-mm-dd hh24:mi:ss") == 0)
  {
    snprintf(stime,20,"%04u-%02u-%02u %02u:%02u:%02u",sttm.tm_year,
                    sttm.tm_mon,sttm.tm_mday,sttm.tm_hour,
                    sttm.tm_min,sttm.tm_sec);
    return;
  }

}

//////////////////////////////////////////////////////////////////////////////////////

// 根据绝对路径的文件名或目录名逐级的创建目录。
// pathorfilename：绝对路径的文件名或目录名。
// bisfilename：说明pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，\缺省值为true。
// 返回值：true-创建成功，false-创建失败，如果返回失败，原因有大概有三种情况：1）权限\不足； 2）pathorfilename参数不是合法的文件名或目录名；3）磁盘空间不足。
bool MKDIR(const char *filename,bool bisfilename)
{
  // 检查目录是否存在，如果不存在，逐级创建子目录
  char strPathName[301];

  int ilen=strlen(filename);

  for (int ii=1; ii<ilen;ii++)
  {
    if (filename[ii] != '/') continue;

    memset(strPathName,0,sizeof(strPathName));
    strncpy(strPathName,filename,ii);

    if (access(strPathName,F_OK) == 0) continue;

    if (mkdir(strPathName,0755) != 0) return false;
  }

  if (bisfilename==false)
  {
    if (access(filename,F_OK) != 0)
    {
      if (mkdir(filename,0755) != 0) return false;
    }
  }

  return true;
}


///////////////////////////////////////////////////////////////////////////////////////

// 打开文件。
// FOPEN函数调用fopen库函数打开文件，如果文件名中包含的目录不存在，就创建目录。
// FOPEN函数的参数和返回值与fopen函数完全相同。
// 在应用开发中，用FOPEN函数代替fopen库函数。
FILE *FOPEN(const char *filename,const char *mode)
{
  if (MKDIR(filename) == false) return 0;

  return fopen(filename,mode);
}


///////////////////////////////////////////////////////////////////////////////////////
CLogFile::CLogFile(const long MaxLogSize)
{
  m_tracefp = 0;
  memset(m_filename,0,sizeof(m_filename));
  memset(m_openmode,0,sizeof(m_openmode));
  m_bBackup=true;
  m_bEnBuffer=false;
  m_MaxLogSize=MaxLogSize;
  if (m_MaxLogSize<10) m_MaxLogSize=10;
}

CLogFile::~CLogFile()
{
  Close();
}

void CLogFile::Close()
{
  if (m_tracefp != 0) { fclose(m_tracefp); m_tracefp=0; }

  memset(m_filename,0,sizeof(m_filename));
  memset(m_openmode,0,sizeof(m_openmode));
  m_bBackup=true;
  m_bEnBuffer=false;
}

// 打开日志文件。
// filename：日志文件名，建议采用绝对路径，如果文件名中的目录不存在，就先创建目录。
// openmode：日志文件的打开方式，与fopen库函数打开文件的方式相同，缺省值是"a+"。
// bBackup：是否自动切换，true-切换，false-不切换，在多进程的服务程序中，如果多个进行\共用一个日志文件，bBackup必须为false。
// bEnBuffer：是否启用文件缓冲机制，true-启用，false-不启用，如果启用缓冲区，那么写进\日志文件中的内容不会立即写入文件，缺省是不启用。
bool CLogFile::Open(const char *filename,const char *openmode,bool bBackup,\
                    bool bEnBuffer)
{
  // 如果文件指针是打开的状态，先关闭它。
  Close();

  strcpy(m_filename,filename);
  m_bEnBuffer=bEnBuffer;
  m_bBackup=bBackup;
  if (openmode==0) strcpy(m_openmode,"a+");
  else strcpy(m_openmode,openmode);

 if ((m_tracefp=FOPEN(m_filename,m_openmode)) == 0) return false;

  return true;
}

// 如果日志文件大于100M，就把当前的日志文件备份成历史日志文件，切换成功后清空当前日志\文件的内容。
// 备份后的文件会在日志文件名后加上日期时间。
// 注意，在多进程的程序中，日志文件不可切换，多线的程序中，日志文件可以切换。
bool CLogFile::BackupLogFile()
{
  if (m_tracefp == 0) return false;

  // 不备份
  if (m_bBackup == false) return true;

  fseek(m_tracefp,0L,2);

  if (ftell(m_tracefp) > m_MaxLogSize*1024*1024)
  {
    fclose(m_tracefp); m_tracefp=0;

    char strLocalTime[21];
    memset(strLocalTime,0,sizeof(strLocalTime));
    LocalTime(strLocalTime,"yyyymmddhh24miss");

    char bak_filename[301];
    memset(bak_filename,0,sizeof(bak_filename));
    snprintf(bak_filename,300,"%s.%s",m_filename,strLocalTime);
    rename(m_filename,bak_filename);

    if ((m_tracefp=FOPEN(m_filename,m_openmode)) == 0) return false;
  }

  return true;
}

// 把内容写入日志文件，fmt是可变参数，使用方法与printf库函数相同。
// Write方法会写入当前的时间，WriteEx方法不写时间。
bool CLogFile::Write(const char *fmt,...)
{
  if (m_tracefp == 0) return false;

  if (BackupLogFile() == false) return false;

  char strtime[20]; LocalTime(strtime);

  va_list ap;
  va_start(ap,fmt);
  fprintf(m_tracefp,"%s ",strtime);
  vfprintf(m_tracefp,fmt,ap);
  va_end(ap);

  if (m_bEnBuffer==false) fflush(m_tracefp);
  return true;
}

// 把内容写入日志文件，fmt是可变参数，使用方法与printf库函数相同。
// Write方法会写入当前的时间，WriteEx方法不写时间。
bool CLogFile::WriteEx(const char *fmt,...)
{
  if (m_tracefp == 0) return false;

  va_list ap;
  va_start(ap,fmt);
  vfprintf(m_tracefp,fmt,ap);
  va_end(ap);

  if (m_bEnBuffer==false) fflush(m_tracefp);

  return true;
}



// 删除字符串左边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLChar(char *str,const char chr)
{
  if (str == 0) return;
  if (strlen(str) == 0) return;

  char strTemp[strlen(str)+1];

  int iTemp=0;

  memset(strTemp,0,sizeof(strTemp));
  strcpy(strTemp,str);

  while ( strTemp[iTemp] == chr )  iTemp++;

  memset(str,0,strlen(str)+1);

  strcpy(str,strTemp+iTemp);

  return;
}

// 删除字符串右边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteRChar(char *str,const char chr)
{
  if (str == 0) return;
  if (strlen(str) == 0) return;

  int istrlen = strlen(str);

  while (istrlen>0)
  {
    if (str[istrlen-1] != chr) break;

    str[istrlen-1]=0;

    istrlen--;
  }
}

// 删除字符串左右两边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLRChar(char *str,const char chr)
{
  DeleteLChar(str,chr);
  DeleteRChar(str,chr);
}



// 把字符串中的小写字母转换成大写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToUpper(char *str)
{
  if (str == 0) return;

  if (strlen(str) == 0) return;

  int istrlen=strlen(str);

  for (int ii=0;ii<istrlen;ii++)
  {
    if ( (str[ii] >= 'a') && (str[ii] <= 'z') ) str[ii]=str[ii] - 32;
  }
}

void ToUpper(string &str)
{
  if (str.empty()) return;

  char strtemp[str.size()+1];

  memset(strtemp,0,sizeof(strtemp));
  strcpy(strtemp,str.c_str());

  ToUpper(strtemp);

  str=strtemp;

  return;
}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,char *value,\
                  const int ilen)
{
  if(value==0) return false;

  if(ilen>0) memset(value,0,ilen+1);

  char *start=0, *end=0;
  char m_SFieldName[51],m_EFieldName[51];
  
  int m_Namelen=strlen(fieldname);
  memset(m_SFieldName,0,sizeof(m_SFieldName));
  memset(m_EFieldName,0,sizeof(m_EFieldName));

  snprintf(m_SFieldName,50,"<%s>",fieldname);
  snprintf(m_EFieldName,50,"</%s>",fieldname);

  start=0; end=0;

  start = (char *)strstr(xmlbuffer,m_SFieldName);
  
  if(start!=0)
  {
    end = (char *)strstr(start,m_EFieldName);
  }

  if( (start==0) || (end==0))
  {
    return false;
  }

  int m_Valuelen=end-start-m_Namelen-2+1;

  if( (m_Valuelen<=ilen) || (ilen==0)  )
  {
    strncpy(value,start+m_Namelen+2,m_Valuelen-1); value[m_Valuelen-1]=0;
  }
  else
  {
    strncpy(value,start+m_Namelen+2,ilen); value[ilen]=0;
  }

  DeleteLRChar(value,' ');

  return true;

}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,bool *value)
{
  if (value==0) return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    ToUpper(strTemp);  // 转换为大写来判断。
    if (strcmp(strTemp,"TRUE")==0) { (*value)=true; return true; }
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,int  *value)
{
  if(value==0) return false;
  
  (*value)=0;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    (*value) = atoi(strTemp); return true;
  }

  return false;

/*
  int m_value=0
  if(GetXMLBuffer(xmlbuffer,fieldname,m_value)==true  )
  {
    value=(int *)m_value; return true;
  }
  else return false;
*/

}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned int *value)
{
  if (value==0) return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    (*value) = (unsigned int )atoi(strTemp); return true;
  }

  return false;
}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,long *value)
{
  if (value==0) return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    (*value) = atol(strTemp); return true;
  }
 
  return false;
}

bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned long *value)
{
  if (value==0) return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    (*value) = (unsigned long)atol(strTemp); return true;
  }

  return false;
}
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,double *value)
{
  if (value==0) return false;

  (*value) = false;

  char strTemp[51];

  memset(strTemp,0,sizeof(strTemp));

  if (GetXMLBuffer(xmlbuffer,fieldname,strTemp,10) == true)
  {
    (*value) = atof(strTemp); return true;
  }

  return false;
}

