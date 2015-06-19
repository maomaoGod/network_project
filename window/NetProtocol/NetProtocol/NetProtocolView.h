
// NetProtocolView.h : CNetProtocolView ��Ľӿ�
//

#pragma once

#include "resource.h"


class CNetProtocolView : public CFormView
{
protected: // �������л�����
	CNetProtocolView();
	DECLARE_DYNCREATE(CNetProtocolView)

public:
	enum{ IDD = IDD_NETPROTOCOL_FORM };

	// ����
public:
	CNetProtocolDoc* GetDocument() const;

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
	virtual ~CNetProtocolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg LRESULT SockStateUpdate(WPARAM, LPARAM);
	// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
private:
	unsigned appnum=0;
	unsigned uploadrate=0;
	unsigned downloadrate=0;
};

#ifndef _DEBUG  // NetProtocolView.cpp �еĵ��԰汾
inline CNetProtocolDoc* CNetProtocolView::GetDocument() const
{
	return reinterpret_cast<CNetProtocolDoc*>(m_pDocument);
}
#endif

