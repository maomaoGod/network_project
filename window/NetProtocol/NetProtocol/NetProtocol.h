
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

#define maxnum 100				///<定义最大的节点数
#define maxint 999999			///<定义最长的节点间距离

/**
*@class <Edge>
*@author ACM2012
*@note
*	需要添加一个这样的结构，然后把边的个数和每个边的初始节点u,v,长度cost建立一个Edge结构
*/
struct Edge{
	int u, v;
};

/**
*@class <common_data>
*@author ACM2012
*@note
*	定义LS算法路由信息和DV算法路由信息的共同结构
*/
struct common_data{
	int node;				 ///< 节点个数
	int sid;				 ///< 源节点
	int did;				 ///< 目的节点
	int dist[maxnum];		 ///< 表示当前点到源点的最短路径长度
	int pre[maxnum];		 ///< 记录当前点的前一个结点
};

/**
*@class <LS_data>
*@author ACM2012
*@note
*	定义LS算法路由信息结构
*/
struct LS_data{
	common_data LsData;
	int c[maxnum][maxnum];   ///< 记录图的两点间路径长度
};

/**
*@class <DV_data>
*@author ACM2012
*@note
*	定义DV算法路由信息结构
*/
struct DV_data{
	common_data DvData;
	int edgenum;			 ///< 边数
	Edge E[maxnum];			 ///< 边结构数组
};

/**
*@class <Route_info>
*@author ACM2012
*@note
*	定义路由信息结构
*/
struct Route_info{
	Edge E[maxnum];
	int node;
	int edgenum;
	int sid;
	int did;
};


#define iIP	3232241409       ///<对应内部局域网的IP
#define oIP 3232241409       ///<对应外部广域网的IP
//#define Routing_select 1     ///<定义选路算法, 1表示LS算法, 0表示DV算法
//#define end_connect 1        ///<定义客户端还是路由器, 1表示客户端, 0表示路由器 
#define data_info  1         ///<定义数据信息还是路由信息, 1表示数据信息, 0表示路由信息
#define IPN	6                ///<定义节点的个数
#define MAX 0x3f3f3f3f    
#define N 1010

/**
*@class <NAT_translation_table>
*@author ACM2012
*@note
*	定义NAT转换表
*/
struct NAT_translation_table
{
	unsigned int WAN_IP;			 ///<  外网IP
	unsigned short WAN_port;         ///<  外网端口
	unsigned int LAN_IP;			 ///<  内网IP
	unsigned short LAN_port;		 ///<  内网端口
	struct NAT_translation_table* next_p;
};

/**
*@class <In_NAT>
*@author ACM2012
*@note
*	定义NAT协议需要输入信息的结构
*/
struct In_NAT{
	unsigned int SIP;			///<  外网IP
	unsigned short Sport;        ///<  外网端口
	unsigned int DIP;			///<  内网IP
	unsigned short Dport;        ///<  内网端口
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
