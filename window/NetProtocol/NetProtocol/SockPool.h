#pragma once

#include <map>
using namespace std;

#define  TRANSTOIP             WM_USER+1000
#define  IPTOLINK                 WM_USER+1001
#define  LINKSEND               WM_USER+1002
#define  TRANSTOAPP         WM_USER+1003
#define  APPTOTRANS	       WM_USER+1004
#define  IPTOTRANS             WM_USER+1005
#define  LINKTOIP                 WM_USER+1006
#define  APPSEND                WM_USER+1007
#define SOCKSTATEUPDATE WM_USER+1008

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208

typedef struct Manager
{
	int reader;
	int writer;
	unsigned int hid;
	unsigned int cid;
	HANDLE Head;
	HANDLE Cur;
	HANDLE Tail;
} *PM;

struct transstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned int  function;     //绑定端口号
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char *data;                     //数据
};

typedef struct Node
{
	unsigned int FuncID;///< 套接字消息类型
	unsigned int AcceptSockMark;///<指派套接字标识
	unsigned short  srcport;    ///<源端口号
	unsigned short  dstport;    ///<目的端口号
	char srcip[20];                 ///<源ip
	char dstip[20];                 ///<目的ip
	unsigned short  bindport;  ///<绑定端口号
	unsigned int  DataLen;     ///<数据长度
	HANDLE Data;               ///<数据句柄
	HANDLE Next;              ///<下一个节点句柄
}*PN;

struct regstruct{
	int    SockMark;            ///<套接字唯一标识
	TCHAR ReadQueueName[20];///<读链表管理结构名
	TCHAR WriteQueueName[20];///<写链表管理结构名
};

struct Para
{
	void *pthis;
	unsigned int SockMark;
	HANDLE CH;
	HANDLE Queue;
	PM pQueue;
};

struct portsrc    //得到目的端口的数据结构
{
	char srcip[20];
	unsigned short srcport;
	unsigned short dstport;
	bool operator <(const portsrc & other) const
	{
		if (strcmp(srcip, other.srcip) < 0)
			return true;
		else if (strcmp(srcip, other.srcip) >0)
			return false;
		else {
			if (srcport < other.srcport)
				return true;
			else if (srcport>other.srcport)
				return false;
			else {
				if (dstport < other.dstport)
					return true;
				else if (dstport>other.dstport)
					return false;
				return false;
			}
		}
	}
};

class SockPool
{
public:
	SockPool();
	~SockPool();
public:
	void   SendToApp(void *);
private:	
	int nPort=0;
	map <portsrc, unsigned int> SrcPort2ScokMark;
	map <unsigned int, transstruct *> SockMark2SockStruct;
	map <unsigned int, unsigned short>  SockMark2Port;
	map <unsigned short, unsigned int>  Port2SockMark;
	map <unsigned int, CEvent *>SockMark2WEvent;
	map <unsigned int, CEvent *>SockMark2REvent;
	map <unsigned int, bool> SockMark2ReadState;
	map <unsigned int, bool> SockMark2WriteState;
	map  <unsigned int, HANDLE> SockMark2ReadThread;
	map <unsigned int, HANDLE> SockMark2WriteThread;
 private:
	HANDLE Wsemaphore, Rsemaphore, Dsemaphore;
	HANDLE MFile;
	HANDLE SH;
	regstruct *preg;
private:
	static DWORD WINAPI NewConnThread(LPVOID lParam);
	static DWORD WINAPI NewReadThread(LPVOID lParam);
	static DWORD WINAPI NewWriteThread(LPVOID lParam);
	bool   InitalReadQueue(HANDLE, PM &, HANDLE &);
	bool   InitalWriteQueue(HANDLE,PM &);
	bool   PrintLog(LPCTSTR, bool);
	void   ClearNode(PM &pWriteQueue);
	bool   AddToTail(PM &,HANDLE);
	void   Connect();
	void   InitalThreadPara(Para &,HANDLE,HANDLE,PM,unsigned int);
	void   SockDataToNode(PN, unsigned int);
	void   AllocResource(unsigned int SockMark);
	void   ReadSock(HANDLE,unsigned int,HANDLE, PM);
	void   WriteSock(HANDLE,unsigned int,HANDLE, PM);
	bool  CloseSock(unsigned int);
	bool  state = true;
};

