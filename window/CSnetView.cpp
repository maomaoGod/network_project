
// CSnetView.cpp : CCSnetView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CSnet.h"
#endif

#include "CSnetDoc.h"
#include "CSnetView.h"
#include "NetDlg.h"
#include "DllDlg.h"
#include "HtmlDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCSnetView

IMPLEMENT_DYNCREATE(CCSnetView, CView)

BEGIN_MESSAGE_MAP(CCSnetView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DLL, &CCSnetView::OnDll)
	ON_COMMAND(ID_NETPAGE, &CCSnetView::OnNetpage)
	ON_COMMAND(32785, &CCSnetView::OnNetDlg)
END_MESSAGE_MAP()

// CCSnetView 构造/析构

CCSnetView::CCSnetView()
{
	// TODO:  在此处添加构造代码

}

CCSnetView::~CCSnetView()
{
}

BOOL CCSnetView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCSnetView 绘制

void CCSnetView::OnDraw(CDC* /*pDC*/)
{
	CCSnetDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO:  在此处为本机数据添加绘制代码
}


// CCSnetView 打印

BOOL CCSnetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCSnetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void CCSnetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// CCSnetView 诊断

#ifdef _DEBUG
void CCSnetView::AssertValid() const
{
	CView::AssertValid();
}

void CCSnetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCSnetDoc* CCSnetView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCSnetDoc)));
	return (CCSnetDoc*)m_pDocument;
}
#endif //_DEBUG


// CCSnetView 消息处理程序

void CCSnetView::OnDll()
{
	Dll  Dlg;
	Dlg.DoModal();
}


void CCSnetView::OnNetpage()
{
	// TODO:  在此添加命令处理程序代码
	HtmlDlg Dlg;
	Dlg.DoModal();

}




void CCSnetView::OnNetDlg()
{
	// TODO:  在此添加命令处理程序代码
	NetDlg Dlg;
	Dlg.DoModal();
}
