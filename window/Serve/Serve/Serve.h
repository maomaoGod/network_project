
// Serve.h : Serve 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define PRINT              WM_USER + 1000
#define REGISTER       WM_USER + 1001
#define SENDTONPC   WM_USER + 1002
#define DNSADDRADD          WM_USER + 1003

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207
#define  SOCKACCEPT               208

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



// CServeApp:
// 有关此类的实现，请参阅 Serve.cpp
//
class CServeApp : public CWinApp
{
public:
	CServeApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServeApp theApp;
