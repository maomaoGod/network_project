
// NetExp.h : NetExp Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CNetExpApp:
// �йش����ʵ�֣������ NetExp.cpp
//

class CNetExpApp : public CWinApp
{
public:
	CNetExpApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CNetExpApp theApp;
