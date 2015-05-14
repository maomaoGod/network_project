
// MainFrm.cpp : CMainFrame ���ʵ��
//
#pragma once
#include "stdafx.h"
#include "Serve.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void PrintView(CString e);
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_WM_COPYDATA()
	//ON_MESSAGE(REGISTER, Register)
	//ON_MESSAGE(SENDTONPC, SendToNPC)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO:  �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/*LRESULT CMainFrame::Register(WPARAM wparam, LPARAM lparam)
{
	COPYDATASTRUCT mycp;
	protocolwnd = ::FindWindow(NULL, _T("NetProtocol"));
	if (protocolwnd == NULL){
		AfxMessageBox(_T("����Э��δ����"));
	//	DestroyWindow();
		return 0;
	}
	mycp.cbData = NULL;
	mycp.dwData = NULL;
	mycp.lpData =  NULL;
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd),(LPARAM)&mycp);
	return 0;
}

// CMainFrame ��Ϣ�������
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pCopyDataStruct != NULL)
	{
		sockstruct mysock = *((sockstruct *)pCopyDataStruct->lpData);
		TCHAR buf[1000];
		int FuncID = pCopyDataStruct->dwData;
		switch (FuncID)
		{
		case SOCKSEND:
			PrintView((TCHAR *)mysock.data);
			break;
		default:
			break;
		}
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::SendToNPC(WPARAM wparam, LPARAM lparam)
{
	if (protocolwnd == NULL)//ĿǰĬ�����������������
		Register(NULL, NULL);
	if (protocolwnd == NULL)
		return 0;
	COPYDATASTRUCT mycp;
	mycp.dwData = wparam;
	mycp.cbData = sizeof(struct sockstruct);
	mycp.lpData = (void *)lparam;
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
	return 0;
}*/


