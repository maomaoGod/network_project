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
public:
	TEXTMETRIC tm;
	int length;
	int maskline;
protected:
	DECLARE_MESSAGE_MAP()
	CString Cmd;
	CEdit *myedit;
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	void DealEnter();
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


