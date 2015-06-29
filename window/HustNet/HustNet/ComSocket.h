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
* @class <Manager>
* @brief   读写链表管理结构，实现不同进程对同一读写队列的管理
* @author ACM2012
* @note   读写链表管理结构，管理结构为进程共享结构,实现多进程对同一链表的管理。
* @param 读链表进程id, writer写链表进程id,hid 当前链表头序号 ,cid 当前读链表序号 ，
* Head链表头节点句柄，Cur当前读节点句柄，Tail链表尾节点句柄
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
* @class <regstruct>
* @brief   套接字注册结构体，套接字注册师填充该内向共享结构体实现进程间通信
* @author ACM2012
* @note   当创建套接字时均要首先在套接字池中进行注册。套接字池维护着套接字注册结构，
*当创建套接字时，套接字填充注册结构，套接字池接收到填充完成获取填充结构信息，分配相应资源
*并等待下一次套接字注册。
* @param 结构体中主要包含源ip,源端口，目的IP，目的端口，以及相应的控制信息
*/
struct regstruct{
	int    SockMark;            ///<套接字唯一标识
	TCHAR ReadQueueName[20];///<读链表管理结构名
	TCHAR WriteQueueName[20];///<写链表管理结构名
};

/**
* @class <CComSocket>
* @brief   套接字的基类，主要完成套接字的辅助功能
* @author ACM2012
* @note   CComSocket为同步套接字和异步套接字基类，主要完成在CSocket和CAsyncSocket中
*的辅助函数，包括套接字注册和套接字打包函数，将应用层传输的数据打包成标准读写队列节点
*/

class CComSocket
{
public:
	  CComSocket();
	~CComSocket();
private:
     static bool Isfirst; 
	 static int  SockNum;
protected:
	 /** @brief  套接字当前状态*/
     enum  { INIT_FLAG, LISTEN_FLAG, TCP_FLAG, UDP_FLAG, CLOSE_FLAG, DIE_FLAG } sockstate;
	 char srcip[20], dstip[20], csrcip[20], cdstip[20];
	 unsigned short srcport,dstport,csrcport,cdstport;
	 unsigned int SockMark;
	 regstruct myreg;
public:
	 static HANDLE  MFile,Wsemaphore, Rsemaphore, Dsemaphore;  //注册文件和同步信号量
	 static regstruct *preg;
	 /** @brief 套接字注册并获取唯一 标识符*/
	 void        GetSockMark(unsigned int &sockmark,regstruct &myreg);
	 /** @brief 打包应用层传输来的数据成标准结构*/
	 HANDLE PackNode(unsigned int function);
	 /** @brief 打包应用层传输来的数据成标准结构 */
	 HANDLE PackNode(unsigned int function, UINT nSocketPort);
	 /** @brief 打包应用层传输来的数据成标准结构 */
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen);
	 /** @brief 打包应用层传输来的数据成标准结构 */
	 HANDLE PackNode(unsigned int function, LPCTSTR lpszHostAddress, UINT nHostPort);
	 /** @brief 打包应用层传输来的数据成标准结构 */
	 HANDLE PackNode(unsigned int function, CComSocket &rConnectedSocket);
	 /** @brief 打包应用层传输来的数据成标准结构 */
	 HANDLE PackNode(unsigned int function, const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	 /** @brief 输出异常信息到文本*/
	 bool  PrintLog(LPCTSTR, bool);
	 /** @brief 辅助函数char型数据转为tchar */
	 void  char2Tchar(LPCTSTR dst, LPSTR src, int maxlen);
	 /** @brief  辅助函数tchar型数据转化为char*/
	 void  Tchar2char(LPSTR dst, LPCTSTR src);
};

