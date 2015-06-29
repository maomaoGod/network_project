
// MainFrm.h : CMainFrame ��Ľӿ�
//
#pragma once

/**
*@class  <CMainFrame>
*@brief   �����ܴ�����, ��Ҫ��ɽ������ʾ����,
*@author ACM2012
*@note �����ܴ����࣬��Ҫ����������ʾ�Ϳ��ơ�
*/

class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	HWND             protocolwnd;
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// ���ɵ���Ϣӳ�亯��
protected:
	/**@brief �����屻����ʱ����������*/
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg LRESULT Register(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT DNSADDRAdd(WPARAM, LPARAM);
	afx_msg void OnDNSADDR();
};


