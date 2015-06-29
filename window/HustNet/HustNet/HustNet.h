
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
#define ENDINPUT             WM_USER+1008
#define GETCOMMAND      WM_USER+1009
#define GETCMDSTATE      WM_USER+1010
#define GETLINESTATE      WM_USER+1011

/**
*@brief �û��Զ�����Ϣ�Ĳ������ݽṹ
*/
struct  CMD{
	int   ID;
	void *para1;
	void *para2;
};

/**
*CHustNetApp:\n
*�йش����ʵ�֣������ HustNet.cpp
*/

/**
* @class <HustNet>
* @brief    ����ʵ��ƽ̨Ӧ�ó�����
* @author ACM2012
* @note   ����������ɵĸ���δ���޸�
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
