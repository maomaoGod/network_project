
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

struct sockstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  funcID; //socket操作码
	unsigned short	padding;		// blank
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char *data;             //数据
};

struct prostruct   //进程间通信结构体
{
	int FuncID;       //socket操作码        
	int SockMark;  //socket编号
	int AcceptSockMark; //转接的socket编号
	sockstruct  mysock;
};

 
typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define ipMSS 1480                ///< 定义IP数据报最大的分片大小1480

/**
*@class <ip_message>
*@brief 存放IP首部的结构体
*@author ACM2012
*@note
*自己定义的IP报文首部
*/
struct ip_message //定义IP数据报文
{
	unsigned ih_version : 4;            ///< 标识了数据包的IP版本号,一共4位, 0100表示IPv4, 0110表示IPv6
	unsigned ip_hdr_length : 4;         ///< 首部长度, 一般为20字节
	unsigned ih_sever : 8;              ///< 8bit的服务类型
	unsigned ih_data_len : 16;	        ///< 16位数据报长度单位为字节

	unsigned ih_ident : 16;	            ///< 数据报的16bit标识
	unsigned ih_flags : 3;              ///< 数据报的3bit标志
	unsigned ih_offset : 13;	        ///< 数据报的13bit片偏移

	unsigned ih_TTL : 8;                ///< 数据报的寿命
	unsigned ih_protl : 8;	            ///< 数据报的协议, 定义0代表传输层数据, 定义1代表节点相接的路由信息
	unsigned ip_checksum : 16;		    ///< 数据报的首部检验和

	unsigned ih_saddr : 32;		            ///< 32位源IP
	unsigned ih_daddr : 32;		            ///< 32位目的IP
	char data[ipMSS];
	bool operator == (const ip_message &it) const
	{
		if (ih_version != it.ih_version) return false;
		if (ip_hdr_length != it.ip_hdr_length) return false;
		if (ih_sever != it.ih_sever) return false;
		if (ih_data_len != it.ih_data_len) return false;
		if (ih_ident != it.ih_ident) return false;
		if (ih_flags != it.ih_flags) return false;
		if (ih_offset != it.ih_offset) return false;
		if (ih_TTL != it.ih_TTL) return false;
		if (ih_protl != it.ih_protl) return false;
		if (ip_checksum != it.ip_checksum) return false;
		if (ih_saddr != it.ih_saddr) return false;
		if (ih_daddr != it.ih_daddr) return false;
		return true;
	}
	bool operator != (const ip_message &it) const
	{
		return !((*this) == it);
	}
};

/**
*@class <Msg>
*@author ACM2012
*@note
*	定义传输层传给网络层的结构
*/
struct Msg{                     ///<数据
	unsigned int ih_sport;		///< 32位源端口号
	unsigned int ih_dport;		///< 32位目的端口号
	unsigned int sip;			///< 32位源IP
	unsigned int dip;			///< 32位目的IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< 上层协议类型
};

/**
*@class <IP_Msg>
*@author ACM2012
*@note
*	定义网络层传给链路层的结构
*/
struct IP_Msg{
	unsigned int ih_saddr;		///< 32位源IP
	unsigned int ih_daddr;		///< 32位目的IP
	unsigned short ih_len;	    ///< 16位数据报总长度
	char *data;
};

/**
*@class  <CNetProtocolApp>
*@brief  协议栈应用程序类
*@author ACM2012
*@note 本程序为MFC向导生成类，主要完成应用程序的初始化工作
*/

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
