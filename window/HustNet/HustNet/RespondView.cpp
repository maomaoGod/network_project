// RespondView.cpp : 实现文件
//

#include "stdafx.h"
#include "HustNet.h"
#include "RespondView.h"


// RespondView

IMPLEMENT_DYNCREATE(RespondView, CView)

RespondView::RespondView()
{

}

RespondView::~RespondView()
{
}

BEGIN_MESSAGE_MAP(RespondView, CView)
END_MESSAGE_MAP()


// RespondView 绘图

void RespondView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// RespondView 诊断

#ifdef _DEBUG
void RespondView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void RespondView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// RespondView 消息处理程序
