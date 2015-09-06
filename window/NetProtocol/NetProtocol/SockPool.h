#pragma once

#include <map>
using namespace std;
/**
*@class  <SockPool>
*@brief  套接字管理类
*@author ACM2012
*@note
*套接字管理模块,管理所有套接字注册和命令处理,并提供协议栈和套接字的连接接口，
*实现数据在应用程序和协议栈程序的进程通信
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
#define  SOCKDESTORY             209


/**
* @class <Manager>
* @brief   读写链表管理结构，实现不同进程对同一读写队列的管理
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

/**
* @class <transstruct>
* @brief   套接字和传输层数据传输结构
* @author ACM2012
* @note   套接字和传输层数据传输结构，主要将应用层数据传输给传输层
* @param 结构体中主要包含源ip,源端口，目的IP，目的端口，以及相应的控制信息
*/

struct transstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  function;     //绑定端口号
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char *data;                     //数据
};

/**
* @class <Node>
* @brief   进程通信节点结构，进程间通信均使用标准的Node结构
* @author ACM2012
* @note   读写链表管理结构，管理结构为进程共享结构,实现多进程对同一链表的管理。
* @param 结构体中主要包含源ip,源端口，目的IP，目的端口，以及相应的控制信息
*/
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

/**
*@class <regstruct>
* @brief   套接字注册结构体，套接字注册师填充该内向共享结构体实现进程间通信
* @author ACM2012
* @note   当创建套接字时均要首先在套接字池中进行注册。套接字池维护着套接字注册结构，
*当创建套接字时，套接字填充注册结构，套接字池接收到填充完成获取填充结构信息，分配相应资源
*并等待下一次套接字注册。
* @param 结构体中主要包含源ip, 源端口，目的IP，目的端口，以及相应的控制信息
*/

struct regstruct{
	int    SockMark;            ///<套接字唯一标识
	TCHAR ReadQueueName[40];///<读链表管理结构名
	TCHAR WriteQueueName[40];///<写链表管理结构名
	TCHAR CDestoryName[40];
	TCHAR SDestoryName[40];
};

/**
* @class <Para>
* @brief   读写队列传入的参数结构体
* @author ACM2012
* @note   套接字和传输层数据传输结构，主要将应用层数据传输给传输层
* @param 结构体中主要包含源套接字标识符，类指针，读写控制块和读写队列指针
*/
struct Para
{
	void *pthis;
	unsigned int SockMark;
	HANDLE CH;
	HANDLE Queue;
	HANDLE Destory;
	PM pQueue;
};

/**
* @class <portin>
* @brief   自定义端口映射结构体，实现套接字端口的映射
* @author ACM2012
* @note   自定义端口映射结构体，实现套接字端口的映射，在程序中目的端口并不是处理信息的真实端口，
* 可能需要做某种映射
* @param 结构体中主要包含源ip,源端口，目的端口
*/

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
	bool Create(CString user);
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
	map <unsigned int, bool> SockMark2State;
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
	void   InitalThreadPara(Para &, HANDLE, HANDLE, PM, unsigned int,HANDLE &);
	/** @brief  传输层数据转化为队列节点结构*/
	bool   SockDataToNode(PN, unsigned int);
	/** @brief 套接字分配资源*/
	void   AllocResource(unsigned int SockMark);
	/** @brief 读队列线程处理函数 */
	void   ReadSock(HANDLE,unsigned int,HANDLE, PM,HANDLE &);
	/** @brief 写队列线程处理函数 */
	void   WriteSock(HANDLE,unsigned int,HANDLE, PM,HANDLE &);
	/** @brief 套接字关闭函数*/
	bool  CloseSock(unsigned int);
	bool  state = true;
};
