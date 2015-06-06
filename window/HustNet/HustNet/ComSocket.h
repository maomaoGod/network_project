#pragma once
#include "HustNet.h"

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

typedef struct Node
{
	unsigned int FuncID;
	unsigned int SockMark;
	unsigned int AcceptSockMark;
	unsigned short  srcport;   //源端口号
	unsigned short  dstport;   //目的端口号
	char srcip[20];                //源ip
	char dstip[20];                //目的ip
	unsigned short  bindport; //绑定端口号
	unsigned int  DataLen; //数据长度
	HANDLE Data;
	HANDLE Next;
}*PN;

struct regstruct{
	int    SockMark;
	TCHAR ReadQueueName[20];
	TCHAR WriteQueueName[20];
};

typedef struct ConnQueue
{
	char srcip[20];
	char dstip[20];
	unsigned short srcport;
	unsigned short dstport;
}*PC;

class CComSocket
{
public:
	  CComSocket();
	~CComSocket();
private:
      bool flag;
public:
	 static int SockMark;
	 static bool Isfirst;
	 static HANDLE  MFile,Csemaphore, Psemaphore, Ssemaphore;  //注册文件和同步信号量
	 static regstruct *preg;                                      
	 void  GetSockMark(int &sockmark,regstruct &myreg);
protected:
	bool PrintLog(LPCTSTR, bool);
	void  char2Tchar(LPCTSTR dst, LPSTR src, int maxlen);
	void  Tchar2char(LPSTR dst, LPCTSTR src);
};

