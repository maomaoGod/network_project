// RespondView.cpp : 实现文件
//
#include "stdafx.h"
#include "HustNet.h"
#include "RespondView.h"

// RespondView
IMPLEMENT_DYNCREATE(RespondView, CEditView)

RespondView::RespondView()
{
	Res.Empty();
	curline = 0;
}

RespondView::~RespondView()
{
}

BEGIN_MESSAGE_MAP(RespondView, CEditView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_MESSAGE(PRINT,Print)
	ON_MESSAGE(CLEAN,Clean)
END_MESSAGE_MAP()


// RespondView 绘图

void RespondView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
	SetWindowText(Res);
}


// RespondView 诊断

#ifdef _DEBUG
void RespondView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void RespondView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// RespondView 消息处理程序


void RespondView::PrintRp(CString mystr)
{
	Res += _T("Respond: ") + mystr + _T("\r\n");
	SetWindowText(Res);
}	


void RespondView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	// TODO:  在此添加专用代码和/或调用基类
	CClientDC dc(this);
	dc.GetTextMetrics(&tm);
	//GetEditCtrl().SetReadOnly(TRUE);
}


BOOL RespondView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此添加专用代码和/或调用基类
		m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL |ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}


void RespondView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	//return;
	CEditView::OnLButtonDown(nFlags, point);
}


void RespondView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}


void RespondView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

LRESULT RespondView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	PrintRp(mystr);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l,l,false);
	((CEdit *)this)->SetFocus();
	return 0;
}

LRESULT RespondView::Clean(WPARAM wparam, LPARAM lparam)
{
	Res.Empty();
	SetWindowText(Res);
	return 0;
}