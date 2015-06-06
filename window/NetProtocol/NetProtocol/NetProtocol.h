
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
	unsigned short  bindport; //�󶨶˿ں�
	int    datalength;             //���ݳ���
	char srcip[20];                //ԭ��ַip
	char dstip[20];                //Ŀ���ַip
	char data[2048];             //����
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
#define MAXSIZE 1480

/**
*@class <_iphdr>
*@brief ���IP�ײ��Ľṹ��
*@author ACM2012
*@note
*�Լ������IP�����ײ�
*/
struct _iphdr //����IP�ײ� 
{
	unsigned short ih_version;  ///< ��ʶ�����ݰ���IP�汾��,һ��4λ, 0100��ʾIPv4, 0110��ʾIPv6
	unsigned short ih_len;	    ///< 16λ���ݱ�����
	unsigned short ih_ident;	///< ���ݱ���16λ��ʶ
	unsigned short ih_flags;    ///< ���ݱ��ı�־
	unsigned short ih_offset;	///< ���ݱ���Ƭƫ��
	unsigned short ih_protl;	///< ���ݱ���Э��, ����0�����������, ����1����ڵ���ӵ���Ϣ
	unsigned int ih_saddr;		///< 32λԴIP
	unsigned int ih_daddr;		///< 32λĿ��IP
	unsigned int ih_sport;		///< 32λԴ�˿ں�
	unsigned int ih_dport;		///< 32λĿ�Ķ˿ں�
	bool operator == (const _iphdr &it) const
	{
		if (ih_version != it.ih_version) return false;
		if (ih_len != it.ih_len) return false;
		if (ih_ident != it.ih_ident) return false;
		if (ih_flags != it.ih_flags) return false;
		if (ih_offset != it.ih_offset) return false;
		if (ih_protl != it.ih_protl) return false;
		if (ih_saddr != it.ih_saddr) return false;
		if (ih_daddr != it.ih_daddr) return false;
		if (ih_sport != it.ih_sport) return false;
		if (ih_dport != it.ih_dport) return false;
		return true;
	}
	bool operator != (const _iphdr &it) const
	{
		return !((*this) == it);
	}
};

struct Msg{                     ///<����
	unsigned int ih_sport;		///< 32λԴ�˿ں�
	unsigned int ih_dport;		///< 32λĿ�Ķ˿ں�
	unsigned int sip;			///< 32λԴIP
	unsigned int dip;			///< 32λĿ��IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< �ϲ�Э������
};

struct IP_Msg{
	_iphdr *iphdr;			///<ָ��ip�ײ���ָ��
	char data[2048];
};

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
