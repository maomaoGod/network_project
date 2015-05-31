// DNS.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Serve.h"
#include "DNS.h"
#include "afxdialogex.h"


// CDNS �Ի���

IMPLEMENT_DYNAMIC(CDNS, CDialogEx)

CDNS::CDNS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDNS::IDD, pParent)
{

}

CDNS::~CDNS()
{
}

void CDNS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDNS, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDNS::OnBnClickedOk)
END_MESSAGE_MAP()


// CDNS ��Ϣ�������


void CDNS::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString Domain, IPaddr;
	GetDlgItem(IDC_domain)->GetWindowText(Domain);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(IPaddr);
	AfxGetApp()->m_pMainWnd->SendMessage(DNSADD,(WPARAM)&Domain,(LPARAM)&IPaddr);
	CDialogEx::OnOK();
}
