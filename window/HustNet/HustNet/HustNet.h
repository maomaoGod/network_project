
/**
*HustNet.h : HustNet Ӧ�ó������ͷ�ļ�
*/

#pragma once
#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

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
*@brief �û��Զ�����Ϣ�Ĳ������ݽṹ
*/
struct  CMD{
	int   ID;
	void *para1;
	void *para2;
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

/**
*CHustNetApp:\n
*�йش����ʵ�֣������ HustNet.cpp
*/

class CHustNetApp : public CWinApp
{
public:
	CHustNetApp();


/**
*��д
*/
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

/**
*ʵ��
*/
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
extern CHustNetApp theApp;
