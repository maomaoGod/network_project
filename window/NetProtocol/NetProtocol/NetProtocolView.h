
// NetProtocolView.h : CNetProtocolView 类的接口
//

#pragma once

#include "resource.h"


class CNetProtocolView : public CFormView
{
protected: // 仅从序列化创建
	CNetProtocolView();
	DECLARE_DYNCREATE(CNetProtocolView)

public:
	enum{ IDD = IDD_NETPROTOCOL_FORM };

	// 特性
public:
	CNetProtocolDoc* GetDocument() const;

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
	virtual ~CNetProtocolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg LRESULT SockStateUpdate(WPARAM, LPARAM);
	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
private:
	unsigned appnum=0;
	unsigned uploadrate=0;
	unsigned downloadrate=0;
};

#ifndef _DEBUG  // NetProtocolView.cpp 中的调试版本
inline CNetProtocolDoc* CNetProtocolView::GetDocument() const
{
	return reinterpret_cast<CNetProtocolDoc*>(m_pDocument);
}
#endif

