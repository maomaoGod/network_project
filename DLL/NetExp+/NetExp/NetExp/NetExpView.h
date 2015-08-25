
// NetExpView.h : CNetExpView ��Ľӿ�
//

#pragma once

#include "resource.h"


class CNetExpView : public CFormView
{
protected: // �������л�����
	CNetExpView();
	DECLARE_DYNCREATE(CNetExpView)

public:
	enum{ IDD = IDD_NETEXP_FORM };

// ����
public:
	CNetExpDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnInitialUpdate(); // ������һ�ε���

// ʵ��
public:
	virtual ~CNetExpView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInput();
};

#ifndef _DEBUG  // NetExpView.cpp �еĵ��԰汾
inline CNetExpDoc* CNetExpView::GetDocument() const
   { return reinterpret_cast<CNetExpDoc*>(m_pDocument); }
#endif

