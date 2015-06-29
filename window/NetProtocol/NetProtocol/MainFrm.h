// MainFrm.h : CMainFrame ��Ľӿ�
//
#pragma once
#include <map>
#include "CMyIP.h"
#include "TransLayer.h"
#include "my_linker.h"
#include "sockpool.h"
using namespace std;


/**
*@class  <CMainFrame>
*@brief   �����ܴ�����,��Ҫ��ɽ������ʾ����,Э��ջ֮����Ϣ�Ĵ���
*@author ACM2012
*@note �����ܴ����࣬��Ҫ����������ʵ���ڱ������У������������Э��ջ�и�����ͨ�ŵĽӿڣ�
*Э��ջ�и�����ͨ�����ڵ���Ϣ���ƽ���ͨ�ţ��Ӷ�ʵ�ָ�����֮�����Ϣ������ѡ����Ϣ��ԭ�����ڴ�����Ϣ
*����ʹ��֮��Ĺ�ϵ�������������ݵĴ��͸�����ȷ��
*/

class CMainFrame : public CFrameWnd
{

protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// ����
public:

	// ����
public:

public:
	int connsocknum;
	CMyIP IP;
	my_linker  linker;
	SockPool   m_sockpool;
	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	HWND PPwnd;
	int mac_flag;
	unsigned int CurIP;
	// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CToolBar            m_wndToolBar;
	CStatusBar        m_wndStatusBar;
	CWnd   *MyWnd;
	int  numprocess;
	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	/** @brief ����㴦����������ݣ�����Ӧ�ò� */
	afx_msg LRESULT OnTrans2App(WPARAM, LPARAM);
	/** @brief ����㴦����·�����ݣ���������� */
	afx_msg LRESULT OnIP2Trans(WPARAM, LPARAM);
	/** @brief  ��·��ץȡ���ݣ���������� */
	afx_msg LRESULT OnLink2IP(WPARAM, LPARAM);
	/** @brief ��������Ӧ�ò����ݣ���������� */
	afx_msg LRESULT OnTrans2IP(WPARAM, LPARAM);
	/** @brief �������մ�������ݣ����� */
	afx_msg LRESULT OnIP2Link(WPARAM, LPARAM);
	/** @brief ��·�������������ݷ��� */
	afx_msg LRESULT OnLinkSend(WPARAM, LPARAM);
	/** @brief ���ݷ���Ӧ�ò�*/
	afx_msg LRESULT OnAppSend(WPARAM, LPARAM);
	/** @brief ��ʱ����Ϣ������ */
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	/** @brief  ѡ�������Ϊ��ϵͳ*/
	afx_msg void OnClient();
	/** @brief  ѡ�������Ϊ·���� */
	afx_msg void OnRoute();
	/** @brief  ʹ��OSPFѡ·Э��*/
	afx_msg void OnOspf();
	/** @brief  ʹ��RIPѡ·Э�� */
	afx_msg void OnRip();
	/** @brief   MainFrame������ɽ��г�ʼ��*/
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
};
