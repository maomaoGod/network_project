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
	enum  CMDMODE { ROOT, USER }  CMDFLAG;
protected:
	TEXTMETRIC tm;
	CFont   myfont;
	CEdit  *myedit;
	CString Hint;
	int HintPLen;    //Hint屏幕尺寸长度
	int HintSLen;    //Hint字符串长(内存长度)
	int HintLine;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnInitialUpdate();
//	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg  LRESULT  OnSetHint(WPARAM wparam, LPARAM lparam);
    afx_msg  LRESULT  OnTakeOverCmd(WPARAM wparam, LPARAM lparam);
	afx_msg  LRESULT  OnDealCmdOver(WPARAM wparam, LPARAM lparam);
	afx_msg  LRESULT  OnEndInput(WPARAM wparam, LPARAM lparam);
protected:
	void DealEnter();
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};


