
// Serve.h : Serve Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#define PRINT                       WM_USER + 1000
#define REGISTER                WM_USER + 1001
#define SENDTONPC            WM_USER + 1002
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
