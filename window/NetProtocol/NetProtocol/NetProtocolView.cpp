
// NetProtocolView.cpp : CNetProtocolView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NetProtocol.h"
#endif

#include "NetProtocolDoc.h"
#include "NetProtocolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetProtocolView

IMPLEMENT_DYNCREATE(CNetProtocolView, CFormView)

BEGIN_MESSAGE_MAP(CNetProtocolView, CFormView)
	ON_MESSAGE(SOCKSTATEUPDATE, SockStateUpdate)
END_MESSAGE_MAP()

// CNetProtocolView 构造/析构

CNetProtocolView::CNetProtocolView()
: CFormView(CNetProtocolView::IDD)
{
	// TODO:  在此处添加构造代码

}

CNetProtocolView::~CNetProtocolView()
{
}

void CNetProtocolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CNetProtocolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CNetProtocolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	GetDlgItem(IDC_SOCKNUM)->SetWindowText(_T("0"));
	GetDlgItem(IDC_APPNUM)->SetWindowText(_T("0"));
	GetDlgItem(IDC_UPLOAD)->SetWindowText(_T("0 B/s"));
	GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_T("0 B/s"));
}


// CNetProtocolView 诊断

#ifdef _DEBUG
void CNetProtocolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNetProtocolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNetProtocolDoc* CNetProtocolView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetProtocolDoc)));
	return (CNetProtocolDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetProtocolView 消息处理程序

LRESULT CNetProtocolView::SockStateUpdate(WPARAM wparam, LPARAM lparam)
{
	TCHAR Buf[10];
	_itot_s((int)wparam, Buf, 10);
	GetDlgItem(IDC_SOCKNUM)->SetWindowText(Buf);
	return 0;
}