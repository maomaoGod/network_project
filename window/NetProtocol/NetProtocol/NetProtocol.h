
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

struct portsrc    //�õ�Ŀ�Ķ˿ڵ����ݽṹ
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
	int SockMark;  //socketΨһ��ʶ��
	TCHAR readfilename[20];  //socket��
	TCHAR writefilename[20]; //socketд
	TCHAR PSname[20];       //������׼�����͵�Ӧ�ó���
	TCHAR PRname[20];       //Э������� 
	TCHAR PWname[20];      //Э������д
	TCHAR CSname[20];       //Ӧ�ó���������׼������
	TCHAR CRname[20];       //Ӧ�������
	TCHAR CWname[20];      //Ӧ������д
};

struct ObjEvent
{
	HANDLE    RFile;       //Ӧ�ó�����ļ����
	HANDLE    WFile;      //Ӧ�ó���д�ļ���� 
	prostruct    *Rpro;       //ӳ�䵽���ض��ļ�ָ��
	prostruct    *Wpro;      //ӳ�䵽����д�ļ�ָ��
	HANDLE    PSsock;   //Э�����׼��д�ź���
	HANDLE    PRsock;   //Э�������ź���
	HANDLE    PWsock;  //Э�����д�ź���
	HANDLE    CSsock;   //Ӧ�ó���׼��д�ź���
	HANDLE    CRsock;   //Ӧ�ó�����ź��� 
	HANDLE    CWsock;  //Ӧ�ó���д�ź���
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
};

struct Msg{                     ///<����
	unsigned int sip;
	unsigned int dip;
	char data[2048];
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
