// LogView.cpp : 实现文件
//

#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"


// LogView

IMPLEMENT_DYNCREATE(LogView, CView)

LogView::LogView()
{

}

LogView::~LogView()
{
}

BEGIN_MESSAGE_MAP(LogView, CView)
END_MESSAGE_MAP()


// LogView 绘图

void LogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// LogView 诊断

#ifdef _DEBUG
void LogView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void LogView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// LogView 消息处理程序
