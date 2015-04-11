// LogView.cpp : 实现文件
//
#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"

// LogView
IMPLEMENT_DYNCREATE(LogView, CEditView)

LogView::LogView()
{
	Res.Empty();
	curline = 0;
	logcount = 0;
}

LogView::~LogView()
{
}

BEGIN_MESSAGE_MAP(LogView, CEditView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_MESSAGE(PRINT, Print)
END_MESSAGE_MAP()


// LogView 绘图

void LogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
	SetWindowText(Res);
}


// LogView 诊断

#ifdef _DEBUG
void LogView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void LogView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogView 消息处理程序


void LogView::PrintRp(CString mystr)
{
	CString temp;
	temp.Format(_T("log%d:%s\r\n"), logcount++, mystr);
	Res += temp;
	SetWindowText(Res);
}


void LogView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	CClientDC dc(this);
	dc.GetTextMetrics(&tm);
	//GetEditCtrl().SetReadOnly(TRUE);
}


BOOL LogView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}


void LogView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//return;
	CEditView::OnLButtonDown(nFlags, point);
}


void LogView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}


void LogView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT LogView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	PrintRp(mystr);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}