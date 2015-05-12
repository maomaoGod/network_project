
/**
*HustNet.h : HustNet 应用程序的主头文件
*/

#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define CMDVIEW   0
#define RPVIEW   1
#define LOGVIEW   2

#define PRINT                    WM_USER+1000
#define CLEAN                   WM_USER+1001
#define SETTEXT                WM_USER+1002
#define TAKEOVERCMD     WM_USER+1003
#define RELEASECMD       WM_USER+1004
#define SETHINT                WM_USER+1005
#define DEALCMDOVER    WM_USER+1006
#define DISPATCH              WM_USER+1007
#define ENDINPUT              WM_USER+1008
#define GETCOMMAND      WM_USER+1009
#define GETCMDSTATE      WM_USER+1010
#define GETLINESTATE      WM_USER+1011
#define SENDTONPC         WM_USER+1012
#define REGISTER             WM_USER+1013

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208


/**
*@brief 用户自定义消息的参数传递结构
*/
struct  CMD{
	int   ID;
	void *para1;
	void *para2;
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

struct sockstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  bindport; //绑定端口号
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char data[2048];             //数据
};

struct prostruct   //进程间通信结构体
{
	int FuncID;       //socket操作码        
	int SockMark;  //socket编号
	int AcceptSockMark; //转接的socket编号
	sockstruct  mysock;
};

struct regstruct{
	int SockMark;  //socket唯一标识码
	TCHAR readfilename[20];  //socket读
	TCHAR writefilename[20]; //socket写
	TCHAR PSname[20];       //有数据准备发送到应用程序
	TCHAR PRname[20];       //协议申请读 
	TCHAR PWname[20];      //协议申请写
	TCHAR CSname[20];       //应用程序有数据准备发送
	TCHAR CRname[20];       //应用申请读
	TCHAR CWname[20];      //应用申请写
};

struct ObjEvent
{
	HANDLE    RFile;       //应用程序读文件句柄
	HANDLE    WFile;      //应用程序写文件句柄 
	prostruct    *Rpro;       //映射到本地读文件指针
	prostruct    *Wpro;      //映射到本地写文件指针
	HANDLE    PSsock;   //协议程序准备写信号量
	HANDLE    PRsock;   //协议程序读信号量
	HANDLE    PWsock;  //协议程序写信号量
	HANDLE    CSsock;   //应用程序准备写信号量
	HANDLE    CRsock;   //应用程序读信号量 
	HANDLE    CWsock;  //应用程序写信号量
};

/**
*CHustNetApp:\n
*有关此类的实现，请参阅 HustNet.cpp
*/

class CHustNetApp : public CWinApp
{
public:
	CHustNetApp();


/**
*重写
*/
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

/**
*实现
*/
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
extern CHustNetApp theApp;
