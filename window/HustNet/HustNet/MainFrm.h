/**@file
*@brief  MainFrm��
*@date 2015.4.20
*@version <0.1>
*@note
*�����Ķ���
*/
#pragma once
#include "CmdView.h"
#include "LogView.h"
#include "RespondView.h"
/**
*@class  <CMainFrame>
*@brief  ��Ҫ������������Ľ����ܿ���
*@author ACM2012
*@note MainFrm�ǳ���Ŀ���࣬��Ҫ�������������֧�źͿ��ơ�MainFrm��Ҫ��ɽ�����ƣ����ڷָ�,
*��Ϣ��ת�ȹ��ܡ��ڳ�ʼ��MainFrmʱ�����ͻ����ֳ�����ֱ���ʾCmdView��LogView��RespondView,��
*��̬������������ͼ��Ϊ������Ϣ���ݣ���ܸ�����Ϣ����ת������Ϣ���͵�ָ���Ĵ��ڣ���ɶ�Ӧ�Ĳ�����
*�ڴ��ڷ����ػ�ʱ����̬�������ڵĴ�С����Ӧ������ʾ��
*/

class CMainFrame : public CFrameWnd
{

protected:
	/**
	*�������л�����
	*/
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	/**
	*����
	*/
public:

	/**
	*����
	*/
public:

	/**
	*��д
	*/
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	/**
	*ʵ��
	*/
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	/**
	*�ؼ���Ƕ���Ա
	*/
protected:
	CToolBar          m_wndToolBar;
	CStatusBar       m_wndStatusBar;
	LogView          *plog;
	CmdView         *pcmd;
	RespondView   *prespond;
	BOOL               flag;
	BOOL              netflag;
public:
	CSplitterWnd    m_splitter, s_splitter;
protected:
	/**@brief �����屻����ʱ����������*/
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	/**@brief CMainFrame ��Ϣ�������ʵ�ִ��ڷָ�*/
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	/**@brief ����仯ʱ��̬���������С*/
	afx_msg void OnSize(UINT nType, int cx, int cy);
	/*@brief  ���յ�DISPATCH��Ϣʱ��������*/
	afx_msg LRESULT Dispatch(WPARAM wparam, LPARAM lparam);
	/*@brief �ر�������ʱ������*/
	afx_msg void OnClose();
	/*@brief ������������*/
	afx_msg void OnNETSET();
	afx_msg void OnHttp();
};