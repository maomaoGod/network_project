#pragma once

#include <map>
using namespace std;
/**
*@class  <SockPool>
*@brief  套接字管理类
*@author ACM2012
*@note
*套接字管理模块,管理所有套接字注册和命令处理,并提供协议栈和套接字的连接接口，实现数据在应用程序和协议栈程序的进程通信
*/

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
	unsigned short  function;     //绑定端口号
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

struct portin    //得到目的端口的数据结构
{
	char srcip[20];
	unsigned short srcport;
	unsigned short dstport;
	bool operator <(const portin & other) const
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
	int     sockconnum;
private:
	int nPort = 0;
	map <portin, unsigned int> PortIn2ScokMark;
	map <unsigned short, unsigned short> Port2PortOut;
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
	/** @brief 套接字注册同步信号量*/
	HANDLE Wsemaphore, Rsemaphore, Dsemaphore;
	/** @brief 套接字文件句柄*/
	HANDLE MFile;
	HANDLE SH;
	regstruct *preg;
private:
	/** @brief 套接字注册线程*/
	static DWORD WINAPI NewConnThread(LPVOID lParam);
	/** @brief 套接字读队列线程 */
	static DWORD WINAPI NewReadThread(LPVOID lParam);
	/** @brief 套接字写队列线程 */
	static DWORD WINAPI NewWriteThread(LPVOID lParam);
	/** @brief  初始化读队列*/
	bool   InitalReadQueue(HANDLE, PM &, HANDLE &);
	/** @brief 初始化写队列 */
	bool   InitalWriteQueue(HANDLE, PM &);
	/** @brief 异常输出函数*/
	bool   PrintLog(LPCTSTR, bool);
	/** @brief 清除写队列中已被读取节点 */
	void   ClearNode(PM &pWriteQueue);
	/** @brief 添加节点到写队列尾部 */
   bool   AddToTail(PM &, HANDLE);
	/** @brief  套接字注册链接处理函数 */
	void   Connect();
	/** @brief 初始化读写线程参数函数 */
	void   InitalThreadPara(Para &, HANDLE, HANDLE, PM, unsigned int);
	/** @brief  传输层数据转化为队列节点结构*/
	void   SockDataToNode(PN, unsigned int);
	/** @brief 套接字分配资源*/
	void   AllocResource(unsigned int SockMark);
	/** @brief 读队列线程处理函数 */
	void   ReadSock(HANDLE,unsigned int,HANDLE, PM);
	/** @brief 写队列线程处理函数 */
	void   WriteSock(HANDLE,unsigned int,HANDLE, PM);
	/** @brief 套接字关闭函数*/
	bool  CloseSock(unsigned int);
	bool  state = true;
};
