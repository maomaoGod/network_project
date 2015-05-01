
/**
*HustNet.h : HustNet 应用程序的主头文件
*/

#pragma once
#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

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
#define ENDINPUT             WM_USER+1008
#define GETCOMMAND      WM_USER+1009
#define GETCMDSTATE      WM_USER+1010
#define GETLINESTATE      WM_USER+1011
#define SENDOUT             WM_USER+1012
#define REGISTER            WM_USER+1013

/**
*@brief 用户自定义消息的参数传递结构
*/
struct  CMD{
	int   ID;
	void *para1;
	void *para2;
};

/**
*CHustNetApp:\n
*有关此类的实现，请参阅 HustNet.cpp
*/

class CHustNetApp : public CWinApp
{
public:
	CHustNetApp();


/**
*重写
*/
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

/**
*实现
*/
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHustNetApp theApp;
