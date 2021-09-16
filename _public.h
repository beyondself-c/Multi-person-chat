/*
 * 程序名：_public.h，公共功能函数声明的头文件
*/
#include "_cmpublic.h"

// 用于把数字的字符转换为整数，chr为用字符方式表示的数字，函数的返回值为数字的整数。
// 如果chr不是数字的字符，则返回-1。
int ctoi(const char chr);

// 求x的y次幂，函数返回值为x的y次幂。
unsigned long  POW(const unsigned int x,unsigned const int y);

//double四舍五入
double ROUND(const double x);

// double向上取整数 
double CEIL(double x);           


// double向下取整数
double FLOOR(double x);               

// 把双精度val分解成整数部分和小数部分，整数部分存放在ip所指的变量中，返回小数部分。
double MODF(double val,double *ip);


class CFile
{
private:

  FILE *m_fp;             //启用文件指针。
  bool m_bEnBuffer;       //是否启用缓冲区，true-启用；false-不启用

public:

   CFile();                //构造函数
   CFile(bool m_bEnBuffer);

   ~CFile();                //析构函数

   void EnBuffer(bool bEnBuffer=true);   // 启、禁用缓冲区

   // 打开文件，参数与fopen相同，打开成功true，失败返回false
   bool Open(const char *filename,const char *filemode);

   //调用fprintf向文件写入数据
   void Fprintf(const char *fmt, ...);

   // 调用fgets从文件中读取一行
   bool Fget(char *strBuffer,const int readsize);

   //关闭文件指针
   void Close();

};

class CRang
{
public:
   vector<int> m_val;  // 用于存放生成好的随机数
   void Rang(const int minvalue,const int maxvalue,const int totalcount,\
             const bool brep);
};

// 以下是socket通信的函数和类

class CTcpClient
{
public:
  int m_sockfd;       //客户端的sockfd
  char m_ip[21];      //服务端的ip
  int m_port;         //与服务端的通信端口
  bool m_btimeout;    //调用Read和Write方法时，失败的原因是否是超时：
                      //true-未超时，false-已超时。

  int m_buflen;       //调用Read方法后，接收到的报文的大小，单位：字节。

  CTcpClient();       //构造函数

  // 向服务端发起请求连接
  // ip：服务端的ip
  // port：服务端监听的端口
  // 返回值：成功返回true，失败返回false
  bool ConnectToServer(const char *ip,const int port);

  // 接收服务端发送过来的数据。
  // buffer：接收数据缓冲区的地址，数据的长度存放在m_buflen成员变量中。
  // itimeout：等待数据的超时时间，单位：秒，缺省值是0-无限等待。
  // 返回值：true-成功；false-失败，失败有两种情况：1）等待超时，成员变量m_btimeout的值
  //被设置为true；2）socket连接已不可用。
  bool Read(char *buffer,const int itimeout);

  // 向服务端发送数据。
  // buffer：待发送数据缓冲区的地址。
  // ibuflen：待发送数据的大小，单位：字节，缺省值为0，如果发送的是ascii字符串，ibuflen取0，如果是二进制流数据，ibuflen为二进制数据块的大小。
  // 返回值：true-成功；false-失败，如果失败，表示socket连接已不可用。
  bool Write(const char *buffer,const int ibuflen=0);

  //断开与服务端的连接
  void Close();
 
  //析构函数，自动关闭sockfd，释放资源
  ~CTcpClient();

};

//用C++的方法封装socket服务端
class CTcpServer
{
private:
  int m_socklen;                     // 结构体struct sockaddr_in的大小。
  struct sockaddr_in m_clientaddr;   // 客户端的地址信息
  struct sockaddr_in m_servaddr;     // 服务端的地址信息

public:
  int m_listenfd;                    // 服务端用于监听的socket
  int m_connfd;                      // 客户端连上来的socket
  bool m_btimeout;                   // 调用Read和Write方法时，失败的原因是否是超时：true-未超时，false-已超时。
  int m_buflen;                     // 调用Read方法后，接收到的报文的大小，单位：字节。
 
  CTcpServer();                      // 构造函数
 
  // 服务端初始化。
  // port：指定服务端用于监听的端口。
  // 返回值：true-成功；false-失败，一般情况下，只要port设置正确，没有被占用，初始化都>会成功。
  bool InitServer(const unsigned int port);


  // 阻塞等待客户端的连接请求。
  // 返回值：true-有新的客户端已连接上来，false-失败，Accept被中断，如果Accept失败，可>以重新Accept。         
  bool Accept();  // 等待客户端的连接
 
  // 获取客户端的ip地址。
  // 返回值：客户端的ip地址，如"192.168.1.100"。
  char *GetIP();

  // 接收客户端发送过来的数据。
  // buffer：接收数据缓冲区的地址，数据的长度存放在m_buflen成员变量中。
  // itimeout：等待数据的超时时间，单位：秒，缺省值是0-无限等待。
  // 返回值：true-成功；false-失败，失败有两种情况：1）等待超时，成员变量m_btimeout的值
  // 被设置为true；2）socket连接已不可用。
  bool Read(char *buffer,const int itimeout);

  // 向客户端发送数据。
  // buffer：待发送数据缓冲区的地址。
  // ibuflen：待发送数据的大小，单位：字节，缺省值为0，如果发送的是ascii字符串，ibuflen  //取0，如果是二进制流数据，ibuflen为二进制数据块的大小。
  // 返回值：true-成功；false-失败，如果失败，表示socket连接已不可用。
  bool Write(const char *buffer,const int ibuflen=0);

  // 关闭监听的socket，即m_listenfd，常用于多进程服务程序的子进程代码中
  void CloseListen();

  // 关闭客户端的socket，即m_connfd，常用于多进程服务程序的父进程代码中。
  void CloseClient();

  ~CTcpServer();  // 析构函数自动关闭socket，释放资源。
};

// 接收socket的对端发送过来的数据。
// sockfd：可用的socket连接。
// buffer：接收数据缓冲区的地址。
// ibuflen：本次成功接收数据的字节数。
// itimeout：接收等待超时的时间，单位：秒，缺省值是0-无限等待。
// 返回值：true-成功；false-失败，失败有两种情况：1）等待超时；2）socket连接已不可用。
bool TcpRead(const int sockfd,char *buffer,int *ibuflen,const int itimeout=0);

// 向socket的对端发送数据。
// sockfd：可用的socket连接。
// buffer：待发送数据缓冲区的地址。
// ibuflen：待发送数据的字节数，如果发送的是ascii字符串，ibuflen取0，如果是二进制流数据，ibuflen为二进制数据块的大小。
// 返回值：true-成功；false-失败，如果失败，表示socket连接已不可用。
bool TcpWrite(const int sockfd,const char *buffer,const int ibuflen=0);

// 从已经准备好的socket中读取数据。
// sockfd：已经准备好的socket连接。
// buffer：接收数据缓冲区的地址。
// n：本次接收数据的字节数。
// 返回值：成功接收到n字节的数据后返回true，socket连接不可用返回false。
bool Readn(const int sockfd,char *buffer,const size_t n);

// 向已经准备好的socket中写入数据。
// sockfd：已经准备好的socket连接。
// buffer：待发送数据缓冲区的地址。
// n：待发送数据的字节数。
// 返回值：成功发送完n字节的数据后返回true，socket连接不可用返回false。
bool Writen(const int sockfd,const char *buffer,const size_t n);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 以下是日志文件操作类

// 日志文件操作类
class CLogFile
{
public:
  FILE   *m_tracefp;           // 日志文件指针。
  char    m_filename[301];     // 日志文件名，建议采用绝对路径。
  char    m_openmode[11];      // 日志文件的打开方式，一般采用"a+"。
  bool    m_bEnBuffer;         // 写入日志时，是否启用操作系统的缓冲机制，缺省不启用。
  long    m_MaxLogSize;        // 最大日志文件的大小，单位M，缺省100M。
  bool    m_bBackup;           // 是否自动切换，日志文件大小超过m_MaxLogSize将自动切换>
                               // 缺省启用。

  // 构造函数。
  // MaxLogSize：最大日志文件的大小，单位M，缺省100M，最小为10M。
  CLogFile(const long MaxLogSize=100);

  // 打开日志文件。
  // filename：日志文件名，建议采用绝对路径，如果文件名中的目录不存在，就先创建目录。
  // openmode：日志文件的打开方式，与fopen库函数打开文件的方式相同，缺省值是"a+"。
  // bBackup：是否自动切换，true-切换，false-不切换，在多进程的服务程序中，如果多个进\程共用一个日志文件，bBackup必须为false。
  // bEnBuffer：是否启用文件缓冲机制，true-启用，false-不启用，如果启用缓冲区，那么写\进日志文件中的内容不会立即写入文件，缺省是不启用。
  bool Open(const char *filename,const char *openmode=0,bool bBackup=true,\
           bool bEnBuffer=false);

  // 如果日志文件大于m_MaxLogSize的值，就把当前的日志文件名改为历史日志文件名，再创建\ 新的当前日志文件。
  // 备份后的文件会在日志文件名后加上日期时间，如/tmp/log/filetodb.log.20200101123025。
  // 注意，在多进程的程序中，日志文件不可切换，多线的程序中，日志文件可以切换。
  bool BackupLogFile();

  // 把内容写入日志文件，fmt是可变参数，使用方法与printf库函数相同。
  // Write方法会写入当前的时间，WriteEx方法不写时间。
  bool Write(const char *fmt,...);
  bool WriteEx(const char *fmt,...);

  // 关闭日志文件
  void Close();

  ~CLogFile();  // 析构函数会调用Close方法。
};



// 根据绝对路径的文件名或目录名逐级的创建目录。
// pathorfilename：绝对路径的文件名或目录名。
// bisfilename：说明pathorfilename的类型，true-pathorfilename是文件名，否则是目录名，\缺省值为true。
// 返回值：true-创建成功，false-创建失败，如果返回失败，原因有大概有三种情况：1）权限\不足； 2）pathorfilename参数不是合法的文件名或目录名；3）磁盘空间不足。
bool MKDIR(const char *filename,bool bisfilename=true);


///////////////////////////////////////////////////////////////////////////////////////////////////

// 打开文件。
// FOPEN函数调用fopen库函数打开文件，如果文件名中包含的目录不存在，就创建目录。
// FOPEN函数的参数和返回值与fopen函数完全相同。
// 在应用开发中，用FOPEN函数代替fopen库函数。
FILE *FOPEN(const char *filename,const char *mode);

///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////

// 把整数表示的时间转换为字符串表示的时间。
// ltime：整数表示的时间。
// stime：字符串表示的时间。
// fmt：输出字符串时间stime的格式，与LocalTime函数的fmt参数相同，如果fmt的格式不正确，\stime将为空。
void timetostr(const time_t ltime,char *stime,const char *fmt);

///////////////////////////////////////////////////////////////////////////////////////

/*
  取操作系统的时间，并把整数表示的时间转换为字符串表示的格式。
  stime：用于存放获取到的时间字符串。
  timetvl：时间的偏移量，单位：秒，0是缺省值，表示当前时间，30表示当前时间30秒之后的时>
间点，-30表示当前时间30秒之前的时间点。
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
void LocalTime(char *stime,const char *fmt=0,const int timetvl=0);



// 删除字符串左边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLChar(char *str,const char chr);

// 删除字符串右边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteRChar(char *str,const char chr);



// 删除字符串左右两边指定的字符。
// str：待处理的字符串。
// chr：需要删除的字符。
void DeleteLRChar(char *str,const char chr);

// 把字符串中的小写字母转换成大写，忽略不是字母的字符。
// str：待转换的字符串，支持char[]和string两种类型。
void ToUpper(char *str);
void ToUpper(string &str);


///////////////////////////////////// /////////////////////////////////////
// 解析xml格式字符串的函数族。
// xml格式的字符串的内容如下：
// <filename>/tmp/freecplus.h</filename><mtime>2020-01-01 12:20:35</mtime><size>18348</// size>
// <filename>/tmp/freecplus.cpp</filename><mtime>2020-01-01 10:10:15</mtime><size>50945// </size>
// xmlbuffer：待解析的xml格式字符串。
// fieldname：字段的标签名。
// value：传入变量的地址，用于存放字段内容，支持bool、int、insigned int、long、unsigned// long、double和char[]。
// 注意，当value参数的数据类型为char []时，必须保证value数组的内存足够，否则可能发生内\存溢出的问题，也可以用ilen参数限定获取字段内容的长度，ilen的缺省值为0，表示不限长度。
// 返回值：true-成功；如果fieldname参数指定的标答名不存在，返回失败。
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,bool *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,int  *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned int *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,long *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,unsigned long *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,double *value);
bool GetXMLBuffer(const char *xmlbuffer,const char *fieldname,char *value,const int ilen=0);
///////////////////////////////////// /////////////////////////////////////


