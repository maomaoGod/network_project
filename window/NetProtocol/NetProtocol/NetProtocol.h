
// NetProtocol.h : NetProtocol Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#define PRINT		         WM_USER+1000
#define CHECKHWND	 WM_USER+1001

// CNetProtocolApp:
// �йش����ʵ�֣������ NetProtocol.cpp
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
	unsigned short  dstport;   //Ŀ�Ķ˿ں�
	unsigned short  srcport;   //Դ�˿ں�
	unsigned short  funcID; //socket������
	int    datalength;             //���ݳ���
	char srcip[20];                //ԭ��ַip
	char dstip[20];                //Ŀ���ַip
	char *data;             //����
};

struct prostruct   //���̼�ͨ�Žṹ��
{
	int FuncID;       //socket������        
	int SockMark;  //socket���
	int AcceptSockMark; //ת�ӵ�socket���
	sockstruct  mysock;
};

 
typedef unsigned char Byte;
typedef unsigned short Ushort;
typedef int Bool;
#define ipMSS 1480                ///< ����IP���ݱ����ķ�Ƭ��С1480

/**
*@class <ip_message>
*@brief ���IP�ײ��Ľṹ��
*@author ACM2012
*@note
*�Լ������IP�����ײ�
*/
struct ip_message //����IP���ݱ���
{
	unsigned ih_version : 4;            ///< ��ʶ�����ݰ���IP�汾��,һ��4λ, 0100��ʾIPv4, 0110��ʾIPv6
	unsigned ip_hdr_length : 4;         ///< �ײ�����, һ��Ϊ20�ֽ�
	unsigned ih_sever : 8;              ///< 8bit�ķ�������
	unsigned ih_data_len : 16;	        ///< 16λ���ݱ����ȵ�λΪ�ֽ�

	unsigned ih_ident : 16;	            ///< ���ݱ���16bit��ʶ
	unsigned ih_flags : 3;              ///< ���ݱ���3bit��־
	unsigned ih_offset : 13;	        ///< ���ݱ���13bitƬƫ��

	unsigned ih_TTL : 8;                ///< ���ݱ�������
	unsigned ih_protl : 8;	            ///< ���ݱ���Э��, ����0�����������, ����1����ڵ���ӵ�·����Ϣ
	unsigned ip_checksum : 16;		    ///< ���ݱ����ײ������

	unsigned ih_saddr : 32;		            ///< 32λԴIP
	unsigned ih_daddr : 32;		            ///< 32λĿ��IP
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
*	���崫��㴫�������Ľṹ
*/
struct Msg{                     ///<����
	unsigned int ih_sport;		///< 32λԴ�˿ں�
	unsigned int ih_dport;		///< 32λĿ�Ķ˿ں�
	unsigned int sip;			///< 32λԴIP
	unsigned int dip;			///< 32λĿ��IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< �ϲ�Э������
};

/**
*@class <IP_Msg>
*@author ACM2012
*@note
*	��������㴫����·��Ľṹ
*/
struct IP_Msg{
	unsigned int ih_saddr;		///< 32λԴIP
	unsigned int ih_daddr;		///< 32λĿ��IP
	unsigned short ih_len;	    ///< 16λ���ݱ��ܳ���
	char *data;
};

#define maxnum 100				///<�������Ľڵ���
#define maxint 999999			///<������Ľڵ�����

/**
*@class <Edge>
*@author ACM2012
*@note
*	��Ҫ���һ�������Ľṹ��Ȼ��ѱߵĸ�����ÿ���ߵĳ�ʼ�ڵ�u,v,����cost����һ��Edge�ṹ
*/
struct Edge{
	int u, v;
};

/**
*@class <common_data>
*@author ACM2012
*@note
*	����LS�㷨·����Ϣ��DV�㷨·����Ϣ�Ĺ�ͬ�ṹ
*/
struct common_data{
	int node;				 ///< �ڵ����
	int sid;				 ///< Դ�ڵ�
	int did;				 ///< Ŀ�Ľڵ�
	int dist[maxnum];		 ///< ��ʾ��ǰ�㵽Դ������·������
	int pre[maxnum];		 ///< ��¼��ǰ���ǰһ�����
};

/**
*@class <LS_data>
*@author ACM2012
*@note
*	����LS�㷨·����Ϣ�ṹ
*/
struct LS_data{
	common_data LsData;
	int c[maxnum][maxnum];   ///< ��¼ͼ�������·������
};

/**
*@class <DV_data>
*@author ACM2012
*@note
*	����DV�㷨·����Ϣ�ṹ
*/
struct DV_data{
	common_data DvData;
	int edgenum;			 ///< ����
	Edge E[maxnum];			 ///< �߽ṹ����
};

/**
*@class <Route_info>
*@author ACM2012
*@note
*	����·����Ϣ�ṹ
*/
struct Route_info{
	Edge E[maxnum];
	int node;
	int edgenum;
	int sid;
	int did;
};


#define iIP	3232241409       ///<��Ӧ�ڲ���������IP
#define oIP 3232241409       ///<��Ӧ�ⲿ��������IP
//#define Routing_select 1     ///<����ѡ·�㷨, 1��ʾLS�㷨, 0��ʾDV�㷨
//#define end_connect 1        ///<����ͻ��˻���·����, 1��ʾ�ͻ���, 0��ʾ·���� 
#define data_info  1         ///<����������Ϣ����·����Ϣ, 1��ʾ������Ϣ, 0��ʾ·����Ϣ
#define IPN	6                ///<����ڵ�ĸ���
#define MAX 0x3f3f3f3f    
#define N 1010

/**
*@class <NAT_translation_table>
*@author ACM2012
*@note
*	����NATת����
*/
struct NAT_translation_table
{
	unsigned int WAN_IP;			 ///<  ����IP
	unsigned short WAN_port;         ///<  �����˿�
	unsigned int LAN_IP;			 ///<  ����IP
	unsigned short LAN_port;		 ///<  �����˿�
	struct NAT_translation_table* next_p;
};

/**
*@class <In_NAT>
*@author ACM2012
*@note
*	����NATЭ����Ҫ������Ϣ�Ľṹ
*/
struct In_NAT{
	unsigned int SIP;			///<  ����IP
	unsigned short Sport;        ///<  �����˿�
	unsigned int DIP;			///<  ����IP
	unsigned short Dport;        ///<  �����˿�
};

/**
*@class  <CNetProtocolApp>
*@brief  Э��ջӦ�ó�����
*@author ACM2012
*@note ������ΪMFC�������࣬��Ҫ���Ӧ�ó���ĳ�ʼ������
*/

class CNetProtocolApp : public CWinApp
{
public:
	CNetProtocolApp();

	// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNetProtocolApp theApp;
