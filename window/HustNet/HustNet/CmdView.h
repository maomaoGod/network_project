#pragma once
#include "MainFrm.h"
#include "LogView.h"
#include "RespondView.h"
// CmdView 视图

class CmdView : public CEditView
{
	DECLARE_DYNCREATE(CmdView)
protected:
	CmdView();           // 动态创建所使用的受保护的构造函数
	virtual ~CmdView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
public:

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};


