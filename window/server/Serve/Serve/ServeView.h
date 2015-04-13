
// ServeView.h : CServeView 类的接口
//

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

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // ServeView.cpp 中的调试版本
inline CServeDoc* CServeView::GetDocument() const
   { return reinterpret_cast<CServeDoc*>(m_pDocument); }
#endif

