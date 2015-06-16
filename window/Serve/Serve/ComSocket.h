#pragma once

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208


#define SOCK_WRONG              1000
#define SOCK_NOT_UDP            1001
#define SOCK_NOT_TCP             1002
#define SOCK_NOT_LISTEN        1003
#define SOCK_IS_CLOSED         1004
#define SOCK_LS_FAILED           1005
/**
* @author ACM2012
* @note   读写链表管理结构，管理结构为进程共享结构,实现多进程对同一链表的管理。
* @param reader 读链表进程id, writer写链表进程id,hid 当前链表头序号 ,cid 当前读链表序号 ，Head链表头节点句柄，Cur当前读节点句柄，Tail链表尾节点句柄
*/
typedef struct Manager
{
	int reader;  ///<读链表进程id
	int writer;   ///<写链表进程id
	unsigned int hid; ///<当前链表头序号
	unsigned int cid;///<当前读链表序号
	HANDLE Head;///<链表头节点句柄
	HANDLE Cur;///<当前读节点句柄
	HANDLE Tail;///<链表尾节点句柄
} *PM;

typedef struct Node
{
	unsigned int FuncID;              ///< 套接字消息类型
	unsigned int AcceptSockMark;///<指派套接字标识
	unsigned short  srcport;    ///<源端口号
	unsigned short  dstport;    ///<目的端口号
	char srcip[20];                 ///<源ip
	char dstip[20];                 ///<目的ip
	unsigned short  bindport;  ///<绑定端口号
	unsigned int  DataLen;     ///<数据长度
	HANDLE Data;                ///<数据句柄
	HANDLE Next;                ///<下一个节点句柄
}*PN;

struct regstruct{
	int    SockMark;            ///<套接字唯一标识
	TCHAR ReadQueueName[20];///<读链表管理结构名
	TCHAR WriteQueueName[20];///<写链表管理结构名
};

class CComSocket
{
public:
	  CComSocket();
	~CComSocket();
private:
     static bool Isfirst; 
	 static int  SockNum;
protected:
     enum  { INIT_FLAG, LISTEN_FLAG, TCP_FLAG, UDP_FLAG, CLOSE_FLAG, DIE_FLAG } sockstate;
	 char srcip[20], dstip[20], csrcip[20], cdstip[20];
	 unsigned short srcport,dstport,csrcport,cdstport;
	 unsigned int SockMark;
	 regstruct myreg;
public:
	 static HANDLE  MFile,Wsemaphore, Rsemaphore, Dsemaphore;  //注册文件和同步信号量
	 static regstruct *preg;                                      
	 void        GetSockMark(unsigned int &sockmark,regstruct &myreg);///<填充节点函数，根据不同的参数填充节点
	 HANDLE PackNode(unsigned int function);
	 HANDLE PackNode(unsigned int function, UINT nSocketPort);
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen);
	 HANDLE PackNode(unsigned int function, LPCTSTR lpszHostAddress, UINT nHostPort);
	 HANDLE PackNode(unsigned int function, CComSocket &rConnectedSocket);
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	 bool  PrintLog(LPCTSTR, bool);
	 void  char2Tchar(LPCTSTR dst, LPSTR src, int maxlen);
	 void  Tchar2char(LPSTR dst, LPCTSTR src);
};

