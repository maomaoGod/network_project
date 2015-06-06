// MainFrm.h : CMainFrame 类的接口
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

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)
	// 特性
public:

	// 操作
public:

public:
	CMyIP ip;
	my_linker linker;
	regstruct  *preg;
	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
	// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CToolBar            m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	SockPool           m_sockpool;
	int  numprocess;
	// 生成的消息映射函数
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
