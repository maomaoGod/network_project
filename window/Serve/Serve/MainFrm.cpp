
// MainFrm.cpp : CMainFrame 类的实现
//
#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "DNS.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void PrintView(CString e);
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ADD_DNS, &CMainFrame::OnDns)
	ON_MESSAGE(DNSADD,DnsAdd)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO:  在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/*LRESULT CMainFrame::Register(WPARAM wparam, LPARAM lparam)
{
	COPYDATASTRUCT mycp;
	protocolwnd = ::FindWindow(NULL, _T("NetProtocol"));
	if (protocolwnd == NULL){
		AfxMessageBox(_T("网络协议未开启"));
		DestroyWindow();
		return 0;
	}
	mycp.dwData = NULL;
	mycp.cbData =  NULL;
	mycp.lpData =   NULL;
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
	return 0;
}

// CMainFrame 消息处理程序
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (pCopyDataStruct != NULL)
	{
		LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
		DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
		CString mystr;
		memcpy(mystr.GetBuffer(dwLength / sizeof(TCHAR)), pszText, dwLength);
		mystr.ReleaseBuffer();
		PrintView(mystr);
		SendMessage(SENDOUT, (WPARAM)&mystr);
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::SendOut(WPARAM wparam, LPARAM lparam)
{
	if (protocolwnd == NULL)//目前默认网络服务是启动的
		Register(NULL, NULL);
	if (protocolwnd == NULL)
		return 0;
	CString *sendtext = (CString *)wparam;
	COPYDATASTRUCT mycp;
	mycp.dwData = _getpid();
	mycp.cbData = (*sendtext).GetLength()*sizeof(TCHAR);
	mycp.lpData = (void*)(*sendtext).GetBuffer(0);
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
	return 0;
}*/
LRESULT CMainFrame::DnsAdd(WPARAM wparam, LPARAM lparam)
{
	CString   Domain, IPaddr;
	Domain = *((CString *)wparam);
	IPaddr =   *((CString *)lparam);
	AfxMessageBox(Domain);
	AfxMessageBox(IPaddr);
	return 0;
}


void CMainFrame::OnDns()
{
	// TODO:  在此添加命令处理程序代码
	CDNS mydns;
	mydns.DoModal();
}
