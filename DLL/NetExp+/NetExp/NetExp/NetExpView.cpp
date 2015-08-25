
// NetExpView.cpp : CNetExpView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "NetExp.h"
#endif

#include "NetExpDoc.h"
#include "NetExpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNetExpView

IMPLEMENT_DYNCREATE(CNetExpView, CFormView)

BEGIN_MESSAGE_MAP(CNetExpView, CFormView)
	ON_BN_CLICKED(IDC_INPUT, &CNetExpView::OnBnClickedInput)
END_MESSAGE_MAP()

// CNetExpView ����/����

CNetExpView::CNetExpView()
	: CFormView(CNetExpView::IDD)
{
	// TODO:  �ڴ˴���ӹ������

}

CNetExpView::~CNetExpView()
{
}

void CNetExpView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CNetExpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CFormView::PreCreateWindow(cs);
}

void CNetExpView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CNetExpView ���

#ifdef _DEBUG
void CNetExpView::AssertValid() const
{
	CFormView::AssertValid();
}

void CNetExpView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNetExpDoc* CNetExpView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNetExpDoc)));
	return (CNetExpDoc*)m_pDocument;
}
#endif //_DEBUG


// CNetExpView ��Ϣ�������


void CNetExpView::OnBnClickedInput()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Func myFunc;
	if (!myFunc.GetFile()){
		AfxMessageBox(_T("Error Input!\n"));
		return;
	}
	GetDlgItem(IDC_ADD)->SetWindowText(myFunc.GetAdd());
	GetDlgItem(IDC_DEC)->SetWindowText(myFunc.GetDec());
	GetDlgItem(IDC_MUL)->SetWindowText(myFunc.GetMul());
	GetDlgItem(IDC_DIV)->SetWindowText(myFunc.GetDiv());
}
