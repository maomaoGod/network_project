
// NetProtocol.h : NetProtocol 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define PRINT		         WM_USER+1000
#define CHECKHWND	 WM_USER+1001

// CNetProtocolApp:
// 有关此类的实现，请参阅 NetProtocol.cpp
//

#define  TRANSTOIP      WM_USER+1000
#define  IPTOLINK          WM_USER+1001
#define  LINKSEND        WM_USER+1002
#define  TRANSTOAPP  WM_USER+1003
#define  APPTOTRANS	WM_USER+1004
#define  IPTOTRANS      WM_USER+1005
#define  LINKTOIP          WM_USER+1006
#define  APPSEND         WM_USER+1007

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
struct parastruct
{
	ObjEvent *pEvent;
	void *pClass;
};
 
typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define MAXSIZE 1480

/**
*@class <_iphdr>
*@brief 存放IP首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
struct _iphdr //定义IP首部 
{
	unsigned short ih_version;  ///< 标识了数据包的IP版本号,一共4位, 0100表示IPv4, 0110表示IPv6
	unsigned short ih_len;	    ///< 16位数据报长度
	unsigned short ih_ident;	///< 数据报的16位标识
	unsigned short ih_flags;    ///< 数据报的标志
	unsigned short ih_offset;	///< 数据报的片偏移
	unsigned short ih_protl;	///< 数据报的协议, 定义0代表传输层数据, 定义1代表节点相接的信息
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
};

struct Msg{                     ///<数据
	unsigned int sip;
	unsigned int dip;
	char data[2048];
};

struct IP_Msg{
	_iphdr *iphdr;			///<指向ip首部的指针
	char data[2048];
};

class CNetProtocolApp : public CWinApp
{
public:
	CNetProtocolApp();

	// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNetProtocolApp theApp;
