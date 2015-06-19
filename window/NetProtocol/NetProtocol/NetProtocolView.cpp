
// NetProtocolView.cpp : CNetProtocolView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "NetProtocol.h"
#endif

#include "NetProtocolDoc.h"
#include "NetProtocolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetProtocolView

IMPLEMENT_DYNCREATE(CNetProtocolView, CFormView)

BEGIN_MESSAGE_MAP(CNetProtocolView, CFormView)
	ON_MESSAGE(SOCKSTATEUPDATE, SockStateUpdate)
END_MESSAGE_MAP()

// CNetProtocolView ����/����

CNetProtocolView::CNetProtocolView()
: CFormView(CNetProtocolView::IDD)
{
	// TODO:  �ڴ˴���ӹ������

}

CNetProtocolView::~CNetProtocolView()
{
}

void CNetProtocolView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CNetProtocolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CNetProtocolView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	GetDlgItem(IDC_SOCKNUM)->SetWindowText(_T("0"));
	GetDlgItem(IDC_APPNUM)->SetWindowText(_T("0"));
	GetDlgItem(IDC_UPLOAD)->SetWindowText(_T("0 B/s"));
	GetDlgItem(IDC_DOWNLOAD)->SetWindowText(_T("0 B/s"));
}


// CNetProtocolView ���

#ifdef _DEBUG
void CNetProtocolView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNetProtocolView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNetProtocolDoc* CNetProtocolView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetProtocolDoc)));
	return (CNetProtocolDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetProtocolView ��Ϣ�������

LRESULT CNetProtocolView::SockStateUpdate(WPARAM wparam, LPARAM lparam)
{
	TCHAR Buf[10];
	_itot_s((int)wparam, Buf, 10);
	GetDlgItem(IDC_SOCKNUM)->SetWindowText(Buf);
	return 0;
}