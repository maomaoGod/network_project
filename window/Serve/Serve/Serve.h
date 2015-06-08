
// Serve.h : Serve Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

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



// CServeApp:
// �йش����ʵ�֣������ Serve.cpp
//
class CServeApp : public CWinApp
{
public:
	CServeApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServeApp theApp;
