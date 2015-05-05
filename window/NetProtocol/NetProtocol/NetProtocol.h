
// NetProtocol.h : NetProtocol 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define PRINT (WM_USER+1000)
#define CHECKHWND WM_USER+1001

// CNetProtocolApp:
// 有关此类的实现，请参阅 NetProtocol.cpp
//

#define  TRANSTOIP      WM_USER+1000
#define  IPTOLINK          WM_USER+1001
#define  LINKSEND        WM_USER+1002
#define  TRANSTOAPP  WM_USER+1003
#define  IPTOTRANS      WM_USER+1004
#define  LINKTOIP          WM_USER+1005
#define  APPTOTRANS   WM_USER+1006

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
