
// MainFrm.cpp : CMainFrame ���ʵ��
//
#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "DNSADDR.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern void PrintView(CString e);
// CMainFrame
CString NowUser;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ADD_DNSADDR, &CMainFrame::OnDNSADDR)
    ON_MESSAGE(DNSADDRADD,DNSADDRAdd)
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
		DestroyWindow();
		return 0;
	}
	mycp.dwData = NULL;
	mycp.cbData =  NULL;
	mycp.lpData =   NULL;
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
	return 0;
}

// CMainFrame ��Ϣ�������
BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (pCopyDataStruct != NULL)
	{
		LPCTSTR pszText = (LPCTSTR)(pCopyDataStruct->lpData);
		DWORD   dwLength = (DWORD)(pCopyDataStruct->cbData);
		CString mystr;
		memcpy(mystr.GetBuffer(dwLength / sizeof(TCHAR)), pszText, dwLength);
		mystr.ReleaseBuffer();
		PrintView(mystr);
		SendMessage(SENDOUT, (WPARAM)&mystr);
	}
	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

LRESULT CMainFrame::SendOut(WPARAM wparam, LPARAM lparam)
{
	if (protocolwnd == NULL)//ĿǰĬ�����������������
		Register(NULL, NULL);
	if (protocolwnd == NULL)
		return 0;
	CString *sendtext = (CString *)wparam;
	COPYDATASTRUCT mycp;
	mycp.dwData = _getpid();
	mycp.cbData = (*sendtext).GetLength()*sizeof(TCHAR);
	mycp.lpData = (void*)(*sendtext).GetBuffer(0);
	::SendMessage(protocolwnd, WM_COPYDATA, (WPARAM)(AfxGetApp()->m_pMainWnd), (LPARAM)&mycp);
	return 0;
}*/

LRESULT CMainFrame::DNSADDRAdd(WPARAM wparam, LPARAM lparam)
{
	GetActiveView()->SendMessage(DNSADDRADD,wparam, lparam);
	return 0;
}


void CMainFrame::OnDNSADDR()
{
	// TODO:  �ڴ���������������
	CDNSADDR myDNSADDR;
	myDNSADDR.DoModal();
}
