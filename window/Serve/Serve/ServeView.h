
// ServeView.h : CServeView ��Ľӿ�
//
#include "HttpServeSocket.h"
#include "FTPServeSocket.h"
#include "Cmysocket.h"
#include "CmyTestSocket.h"
#include "SMTPServeSocket.h"
#include "DNSServeSocket.h"
#include "DnsSocket.h"
#pragma once

class CServeView : public CEditView
{
protected: // �������л�����
	CServeView();
	DECLARE_DYNCREATE(CServeView)

// ����
public:
	CServeDoc* GetDocument() const;

// ����
public:
   // CmyTestSocket    mytestsock;
protected :
	CmyTestSocket mytestsock;
	HttpServeSocket  HttpManagerSocket;
	FTPServeSocket FtpManagerSocket;
	SMTPServeSocket SmtpManagerSocket;
	DNSServeSocket DnsManagerSocket;
	//DnsSocket DnsManagerSocket;
// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CServeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CString mylog;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnPrint(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT DNSADDRAdd(WPARAM, LPARAM);
protected:
	void OnPrint(CString e);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

#ifndef _DEBUG  // ServeView.cpp �еĵ��԰汾
inline CServeDoc* CServeView::GetDocument() const
   { return reinterpret_cast<CServeDoc*>(m_pDocument); }
#endif

