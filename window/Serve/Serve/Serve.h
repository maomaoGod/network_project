
// Serve.h : Serve Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

#define PRINT          WM_USER +1000
#define REGISTER   WM_USER +1001
#define SENDOUT    WM_USER +1002

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
