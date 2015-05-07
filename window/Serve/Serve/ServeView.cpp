
// ServeView.cpp : CServeView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Serve.h"
#endif

#include "MainFrm.h"
#include "ServeDoc.h"
#include "ServeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CServeView

IMPLEMENT_DYNCREATE(CServeView, CEditView)

BEGIN_MESSAGE_MAP(CServeView, CEditView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEditView::OnFilePrintPreview)
	ON_WM_CHAR()
	ON_MESSAGE(PRINT,OnPrint)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

// CServeView 构造/析构

void PrintView(CString e)
{
	((CMainFrame *)AfxGetApp()->GetMainWnd())->GetActiveView()->SendMessage(PRINT, (WPARAM)&e);
}

CServeView::CServeView()
{
	// TODO:  在此处添加构造代码
	mylog.Empty();
	AfxSocketInit();
	HttpManagerSocket.Create(6500);
	HttpManagerSocket.Listen();
	FtpManagerSocket.Create(7600);
	FtpManagerSocket.Listen();
}

CServeView::~CServeView()
{
}

BOOL CServeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE;
	//((CEdit *)this)->HideCaret(
	return CCtrlView::PreCreateWindow(cs);
}


// CServeView 打印

BOOL CServeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认 CEditView 准备
	return CEditView::OnPreparePrinting(pInfo);
}

void CServeView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 开始打印
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CServeView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// 默认 CEditView 结束打印
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CServeView 诊断

#ifdef _DEBUG
void CServeView::AssertValid() const
{
	CEditView::AssertValid();
}

void CServeView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CServeDoc* CServeView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CServeDoc)));
	return (CServeDoc*)m_pDocument;
}
#endif //_DEBUG


// CServeView 消息处理程序


void CServeView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}


LRESULT CServeView::OnPrint(WPARAM wparam,LPARAM lprarm)
{
	CString mystr;
	mystr = (*((CString *)wparam)) + _T("\r\n");
	mylog += mystr;
	SetWindowText(mylog);
	return 0;
}


void CServeView::OnSetFocus(CWnd* pOldWnd)
{
	//CEditView::OnSetFocus(pOldWnd);

	// TODO:  在此处添加消息处理程序代码
}


void CServeView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();
	// TODO:  在此添加专用代码和/或调用基类
	AfxGetMainWnd()->SendMessage(REGISTER, NULL, NULL);
	mysock.Bind(6500);
}
