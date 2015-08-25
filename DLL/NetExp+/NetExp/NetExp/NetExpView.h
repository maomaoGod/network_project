
// NetExpView.h : CNetExpView 类的接口
//

#pragma once

#include "resource.h"


class CNetExpView : public CFormView
{
protected: // 仅从序列化创建
	CNetExpView();
	DECLARE_DYNCREATE(CNetExpView)

public:
	enum{ IDD = IDD_NETEXP_FORM };

// 特性
public:
	CNetExpDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CNetExpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInput();
};

#ifndef _DEBUG  // NetExpView.cpp 中的调试版本
inline CNetExpDoc* CNetExpView::GetDocument() const
   { return reinterpret_cast<CNetExpDoc*>(m_pDocument); }
#endif

