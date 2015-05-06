
// Serve.h : Serve 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

#define PRINT              WM_USER + 1000
#define REGISTER       WM_USER + 1001
#define SENDTONPC   WM_USER + 1002

#define  SOCKCONNECT            200
#define  SOCKBIND                    201
#define  SOCKLISTEN                 202
#define  SOCKSEND                   203
#define  SOCKSENDTO               204
#define  SOCKRECEIVE              205

struct sockstruct {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  bindport; //绑定端口号
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char data[2048];             //数据
};


// CServeApp:
// 有关此类的实现，请参阅 Serve.cpp
//

class CServeApp : public CWinApp
{
public:
	CServeApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServeApp theApp;
