
// ServeView.h : CServeView 类的接口
//
#include "HttpServeSocket.h"
#include "FTPServeSocket.h"
#include "SMTPServeSocket.h"
#pragma once

class CServeView : public CEditView
{
protected: // 仅从序列化创建
	CServeView();
	DECLARE_DYNCREATE(CServeView)

// 特性
public:
	CServeDoc* GetDocument() const;

// 操作
public:

protected :
	HttpServeSocket  HttpManagerSocket;
	FTPServeSocket FtpManagerSocket;
	SMTPServeSocket SmtpManagerSocket;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CServeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString mylog;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPrint(WPARAM wparam, LPARAM lparam);
protected:
	void OnPrint(CString e);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // ServeView.cpp 中的调试版本
inline CServeDoc* CServeView::GetDocument() const
   { return reinterpret_cast<CServeDoc*>(m_pDocument); }
#endif

