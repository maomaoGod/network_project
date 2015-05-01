
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
#include "Mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void PrintView(CString e)
{
	((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView()->SendMessage(PRINT, (WPARAM)&e);
}


// CNetProtocolView

IMPLEMENT_DYNCREATE(CNetProtocolView, CEditView)

BEGIN_MESSAGE_MAP(CNetProtocolView, CEditView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
	ON_MESSAGE(PRINT, OnPrint)
//	ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

// CNetProtocolView 构造/析构

CNetProtocolView::CNetProtocolView()
{
	// TODO:  在此处添加构造代码
	log.Empty();
}

CNetProtocolView::~CNetProtocolView()
{
}

BOOL CNetProtocolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// 启用换行

	return bPreCreated;
}


// CNetProtocolView 打印

BOOL CNetProtocolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认 CEditView 准备
	return CEditView::OnPreparePrinting(pInfo);
}

void CNetProtocolView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 开始打印
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CNetProtocolView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 结束打印
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CNetProtocolView 诊断

#ifdef _DEBUG
void CNetProtocolView::AssertValid() const
{
	CEditView::AssertValid();
}

void CNetProtocolView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CNetProtocolDoc* CNetProtocolView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetProtocolDoc)));
	return (CNetProtocolDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetProtocolView 消息处理程序
LRESULT CNetProtocolView::OnPrint(WPARAM wparam, LPARAM lparam)
{
	CString mystr;
	mystr = *((CString *)wparam)+ _T("\r\n");
	log += mystr;
	SetWindowText(log);
	return 0;
}

