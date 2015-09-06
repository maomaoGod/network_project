
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
	unsigned short	padding;		// blank
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
