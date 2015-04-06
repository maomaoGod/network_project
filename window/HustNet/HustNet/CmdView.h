#pragma once


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

protected:
	DECLARE_MESSAGE_MAP()
};


