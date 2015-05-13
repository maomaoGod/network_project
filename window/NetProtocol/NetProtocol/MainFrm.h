// MainFrm.h : CMainFrame ��Ľӿ�
//
#pragma once
#include <map>
#include "CMyIP.h"
#include "TransLayer.h"
#include "my_linker.h"
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
	sockstruct *psock;
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
	CWnd   *MyWnd;
	map <portsrc, unsigned short>       src2port;
	map <int, unsigned short>              SockMark2Port;
	map <unsigned short, prostruct *>   port2Rstruct;
	map <unsigned short, prostruct *>   port2Wstruct;
	map<ObjEvent *, unsigned short>    Sock2Port;
	map<unsigned short, ObjEvent *>    Port2Sock;
	int  numprocess;
	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	static DWORD WINAPI packcap(LPVOID lParam);
    static DWORD WINAPI connect(LPVOID lParam);
	static DWORD WINAPI ReadFromApp(LPVOID lParam);
	static DWORD WINAPI WriteToApp(LPVOID lParam);
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
//	afx_msg LRESULT OnCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnTrans2App(WPARAM, LPARAM);
	afx_msg LRESULT OnApp2Trans(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Trans(WPARAM, LPARAM);
	afx_msg LRESULT OnLink2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnTrans2IP(WPARAM, LPARAM);
	afx_msg LRESULT OnIP2Link(WPARAM, LPARAM);
	afx_msg LRESULT OnLinkSend(WPARAM, LPARAM);
	afx_msg LRESULT OnAppSend(WPARAM, LPARAM);
	//afx_msg void OnSENDHUST();
};
