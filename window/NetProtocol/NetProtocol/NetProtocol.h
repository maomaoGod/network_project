
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
#define  TRANSTOAPP	WM_USER+1003
#define  APPTOTRANS	WM_USER+1004
#define  IPTOTRANS      WM_USER+1005
#define  LINKTOIP          WM_USER+1006

#define  SOCKCONNECT             200
#define  SOCKBIND                     201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205
#define  SOCKRECEIVEFROM    206
#define  SOCKCLOSE                 207

struct sockstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  bindport; //绑定端口号
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char data[2048];             //数据
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
	unsigned int ih_sport;		///< 32位源端口号
	unsigned int ih_dport;		///< 32位目的端口号
};

struct Msg{                     ///<数据
	unsigned int ih_sport;		///< 32位源端口号
	unsigned int ih_dport;		///< 32位目的端口号
	unsigned int sip;			///< 32位源IP
	unsigned int dip;			///< 32位目的IP
	int datelen;
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
