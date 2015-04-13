
// MainFrm.cpp : CMainFrame 类的实现
//
#include "stdafx.h"
#include "HustNet.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(DISPATCH,Dispatch)
	ON_MESSAGE(SHOW, Show)
	ON_WM_CLOSE()
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
	flag = false;
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
	cs.cx = 900;
	cs.cy = 600;
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


// CMainFrame 消息处理程序



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO:  在此添加专用代码和/或调用基类
	CRect rect;
	GetClientRect(&rect);
	//m_spliter.Create(this, 2, 2, CSize(10, 10), pContext);
	BOOL bCreateSplit = m_splitter.CreateStatic(this, 1, 2);
	if (!s_splitter.CreateStatic(&m_splitter, 2, 1, WS_CHILD | WS_VISIBLE, m_splitter.IdFromRowCol(0, 0)))
	{
		return FALSE;
	}
	s_splitter.CreateView(0, 0, RUNTIME_CLASS(CmdView), CSize(rect.Width() / 2, rect.Height() / 2), pContext);
	s_splitter.CreateView(1, 0, RUNTIME_CLASS(LogView), CSize(rect.Width() / 2, rect.Height() / 4), pContext);
	m_splitter.CreateView(0, 1, RUNTIME_CLASS(RespondView), CSize(rect.Width() / 2, rect.Height()), pContext);
	CWnd *pwnd;
	pwnd = this->s_splitter.GetPane(0, 0);
	pcmd = DYNAMIC_DOWNCAST(CmdView, pwnd);
	pwnd = this->s_splitter.GetPane(1, 0);
	plog = DYNAMIC_DOWNCAST(LogView, pwnd);
	pwnd = this->m_splitter.GetPane(0, 1);
	prespond = DYNAMIC_DOWNCAST(RespondView, pwnd);
	flag = true;
	return bCreateSplit;
	//return CFrameWnd::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (flag){
		if (nType != SIZE_MINIMIZED){
			CRect rect;
			GetClientRect(&rect);
			m_splitter.SetRowInfo(0, cy, 100);
			m_splitter.SetColumnInfo(0, rect.Width() / 3, 100);
			s_splitter.SetColumnInfo(0, rect.Width() / 3, 100);
			m_splitter.RecalcLayout();
			s_splitter.RecalcLayout();
		}
	}
}

LRESULT CMainFrame::Dispatch(WPARAM wparam, LPARAM lparam)
{
	int CMD_ID;
	void *CMD_ARGS;
	CMD_ID = ((CMD *)lparam)->ID;
	CMD_ARGS = ((CMD *)lparam)->agrs;
	switch (wparam)
	{
	case CMDVIEW: 
		pcmd->SendMessage(CMD_ID, (WPARAM)CMD_ARGS);
		break;
	case RPVIEW:
		prespond->SendMessage(CMD_ID, (WPARAM)CMD_ARGS);
		break;
	case LOGVIEW: 
		plog->SendMessage(CMD_ID, (WPARAM)CMD_ARGS);
		break;
	default: break;
	}
	return 0;
}

LRESULT CMainFrame::Show(WPARAM wparam, LPARAM lparam)
{
	switch (wparam)
	{
	case RPVIEW:
		prespond->ShowWindow(SW_HIDE);
		break;
	default: break;
	}
	return 0;
}


void CMainFrame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CFrameWnd::OnClose();
}
