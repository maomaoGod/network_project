// DNSADDR.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Serve.h"
#include "DNSADDR.h"
#include "afxdialogex.h"


// CDNSADDR �Ի���

IMPLEMENT_DYNAMIC(CDNSADDR, CDialogEx)

CDNSADDR::CDNSADDR(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDNSADDR::IDD, pParent)
{

}

CDNSADDR::~CDNSADDR()
{
}

void CDNSADDR::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDNSADDR, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDNSADDR::OnBnClickedOk)
END_MESSAGE_MAP()


// CDNSADDR ��Ϣ�������


void CDNSADDR::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString Domain, IPaddr;
	GetDlgItem(IDC_domain)->GetWindowText(Domain);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(IPaddr);
	AfxGetApp()->m_pMainWnd->SendMessage(DNSADDRADD,(WPARAM)&Domain,(LPARAM)&IPaddr);
	CDialogEx::OnOK();
}
