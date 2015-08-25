
// NetExpView.cpp : CNetExpView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "NetExp.h"
#endif

#include "NetExpDoc.h"
#include "NetExpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetExpView

IMPLEMENT_DYNCREATE(CNetExpView, CFormView)

BEGIN_MESSAGE_MAP(CNetExpView, CFormView)
	ON_BN_CLICKED(IDC_INPUT, &CNetExpView::OnBnClickedInput)
END_MESSAGE_MAP()

// CNetExpView 构造/析构

CNetExpView::CNetExpView()
	: CFormView(CNetExpView::IDD)
{
	// TODO:  在此处添加构造代码

}

CNetExpView::~CNetExpView()
{
}

void CNetExpView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CNetExpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CNetExpView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CNetExpView 诊断

#ifdef _DEBUG
void CNetExpView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNetExpView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNetExpDoc* CNetExpView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetExpDoc)));
	return (CNetExpDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetExpView 消息处理程序


void CNetExpView::OnBnClickedInput()
{
	// TODO:  在此添加控件通知处理程序代码
	Func myFunc;
	if (!myFunc.GetFile()){
		AfxMessageBox(_T("Error Input!\n"));
		return;
	}
	GetDlgItem(IDC_ADD)->SetWindowText(myFunc.GetAdd());
	GetDlgItem(IDC_DEC)->SetWindowText(myFunc.GetDec());
	GetDlgItem(IDC_MUL)->SetWindowText(myFunc.GetMul());
	GetDlgItem(IDC_DIV)->SetWindowText(myFunc.GetDiv());
}
