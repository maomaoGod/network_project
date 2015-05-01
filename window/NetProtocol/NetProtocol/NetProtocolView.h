
// NetProtocolView.h : CNetProtocolView 类的接口
//

#pragma once


class CNetProtocolView : public CEditView
{
protected: // 仅从序列化创建
	CNetProtocolView();
	DECLARE_DYNCREATE(CNetProtocolView)

// 特性
public:
	CNetProtocolDoc* GetDocument() const;
	CString  log;
// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	afx_msg LRESULT OnPrint(WPARAM wparam,LPARAM lparam);

// 实现
public:
	virtual ~CNetProtocolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
};

#ifndef _DEBUG  // NetProtocolView.cpp 中的调试版本
inline CNetProtocolDoc* CNetProtocolView::GetDocument() const
   { return reinterpret_cast<CNetProtocolDoc*>(m_pDocument); }
#endif

