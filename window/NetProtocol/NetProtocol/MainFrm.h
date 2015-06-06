// MainFrm.h : CMainFrame ��Ľӿ�
//
#pragma once
#include <map>
#include "CMyIP.h"
#include "TransLayer.h"
#include "my_linker.h"
#include "SockPool.h"

using namespace std;

class CMainFrame : public CFrameWnd
{

protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	// ����
public:

	// ����
public:

public:
	CMyIP ip;
	my_linker linker;
	regstruct  *preg;
	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
	// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar            m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	SockPool           m_sockpool;
	int  numprocess;
	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static DWORD WINAPI packcap(LPVOID lParam);
	static DWORD WINAPI ReadFromApp(LPVOID lParam);
	static DWORD WINAPI WriteToApp(LPVOID lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnAppSend(WPARAM,LPARAM);
	afx_msg LRESULT OnTrans2App(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Trans(WPARAM, LPARAM);
	afx_msg LRESULT OnLink2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnTrans2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Link(WPARAM, LPARAM);
	afx_msg LRESULT OnLinkSend(WPARAM, LPARAM);
	afx_msg LRESULT SockStateUpdate(WPARAM, LPARAM);
};
