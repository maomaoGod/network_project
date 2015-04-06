// CmdView.cpp : 实现文件
//

#include "stdafx.h"
#include "HustNet.h"
#include "CmdView.h"


// CmdView

IMPLEMENT_DYNCREATE(CmdView, CEditView)

CmdView::CmdView()
{

}

CmdView::~CmdView()
{
}

BEGIN_MESSAGE_MAP(CmdView, CEditView)
END_MESSAGE_MAP()


// CmdView 诊断

#ifdef _DEBUG
void CmdView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CmdView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CmdView 消息处理程序
