
// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "NetProtocol.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define SERVE 1000
#define CLIENT 2000
extern void PrintView (CString e);
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_COPYDATA()
	ON_MESSAGE(CHECKHWND,  OnCheck)
	ON_MESSAGE(TRANSTOAPP, OnTrans2App)
	ON_MESSAGE(IPTOTRANS, OnIP2Trans)
	ON_MESSAGE(LINKTOIP, OnLink2IP)
	ON_MESSAGE(TRANSTOIP, OnTrans2IP)
	ON_MESSAGE(IPTOLINK, OnIP2Link)
	ON_MESSAGE(LINKSEND, OnLinkSend)
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
	numprocess = 0;
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
	cs.style &= ~FWS_ADDTOTITLE;
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

BOOL CALLBACK lpEnumHwnd(HWND hwnd, LPARAM lParam)//遍历所有窗口，寻找客户端和服务器程序
{
	CString Client, Serve;
	Client = _T("华中科技大学网络实验平台");
	Serve = _T("NetServe");
	TCHAR str[100];
	::GetWindowText(hwnd,str,100);
	if (Client.Compare(str) == 0 || Serve.Compare(str)==0)
		(AfxGetApp()->m_pMainWnd)->SendMessage(CHECKHWND, (WPARAM)&hwnd,lParam);
	return 1;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
// TODO:  在此添加消息处理程序代码和/或调用默认值
       if (pwnd2port.find(pWnd) == pwnd2port.end()){  //应用程序注册
		   EnumWindows(lpEnumHwnd, (LPARAM)pWnd);
		   return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
       }
      if (pCopyDataStruct != NULL){//接受来自应用程序的消息
           LPCTSTR pszText =    (LPCTSTR)(pCopyDataStruct->lpData);
           DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
           CString mystr;
           memcpy(mystr.GetBuffer(dwLength/sizeof(TCHAR)), pszText, dwLength);
           mystr.ReleaseBuffer();
           PrintView(mystr);
		   HWND swnd = ::FindWindow(NULL,_T("华中科技大学网络实验平台"));
           ::SendMessage(port2hwnd[pwnd2port[pWnd]], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
       }
      return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::OnCheck(WPARAM wparam, LPARAM lparam)
{
	HWND mywnd = *((HWND *)wparam);
     PPwnd = ::FindWindow(NULL, _T("华中科技大学网络实验平台"));
	int index;
	TCHAR str[100];
	::GetWindowText(mywnd, str, 100);
	for (index = 0; index < numprocess;index++)
	  if (port2hwnd[index] == mywnd)
		    break;
	  if (index == numprocess&&pwnd2port.find((CWnd *)lparam)==pwnd2port.end()){ //一个窗口只能注册一次
		pwnd2port[(CWnd *)lparam] = numprocess;
		port2hwnd[numprocess] = mywnd;
		numprocess = numprocess + 1;
	  } 
	return 0;
}

LRESULT OnTrans2App(WPARAM wparam, LPARAM lparam) //传输层解包传输数据到应用层的接口
{ //使用sendmessage向应用程序发送消息
  //example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
  //应用层发往传输层的数据在OnCopyData中获取
	return 0;
}

LRESULT OnIP2Trans(WPARAM wparam, LPARAM lparam) //网络层解包传输到传输层的接口
{ //
	return 0;
}

LRESULT OnLink2IP(WPARAM wparam, LPARAM lparam) //链路层解包传输数据网络层的接口
{//
	return 0;
}

LRESULT OnTrans2IP(WPARAM wparam, LPARAM lparam) //传输层打包数据发送到网络层的接口
{ //使用sendmessage向应用程序发送消息
	//example 向端口号为0的应用程序发送pCopyDataStruct数据  ::SendMessage(port2hwnd[0], WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)pCopyDataStruct);
	//应用层发往传输层的数据在OnCopyData中获取
	return 0;
}

LRESULT OnIP2Link(WPARAM wparam, LPARAM lparam) //网络层打包数据发送到链路层接口
{ //
	return 0;
}

LRESULT OnLinkSend(WPARAM wparam, LPARAM lparam) //链路层打包数据发送出去接口
{//
	return 0;
}






