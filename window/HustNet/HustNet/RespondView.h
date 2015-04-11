#pragma once


// RespondView 视图

class RespondView : public CEditView
{
	DECLARE_DYNCREATE(RespondView)

protected:
	RespondView();           // 动态创建所使用的受保护的构造函数
	virtual ~RespondView();
public:
	CString  Res;
public:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	int  curline;
	TEXTMETRIC tm;
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT Print(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT Clean(WPARAM wparam, LPARAM lparam);
public:
	void PrintRp(CString mystr);
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


