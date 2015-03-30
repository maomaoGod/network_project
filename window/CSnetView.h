
// CSnetView.h : CCSnetView 类的接口
//

#pragma once


class CCSnetView : public CView
{
protected: // 仅从序列化创建
	CCSnetView();
	DECLARE_DYNCREATE(CCSnetView)

// 特性
public:
	CCSnetDoc* GetDocument() const;

// 操作

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCSnetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDll();
	afx_msg void OnNetpage();
	afx_msg void OnNetDlg();
};

#ifndef _DEBUG  // CSnetView.cpp 中的调试版本
inline CCSnetDoc* CCSnetView::GetDocument() const
   { return reinterpret_cast<CCSnetDoc*>(m_pDocument); }
#endif

