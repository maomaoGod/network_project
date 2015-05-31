// DNS.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DNS.h"
#include "afxdialogex.h"


// CDNS 对话框

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


// CDNS 消息处理程序


void CDNS::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Domain, IPaddr;
	GetDlgItem(IDC_domain)->GetWindowText(Domain);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(IPaddr);
	AfxGetApp()->m_pMainWnd->SendMessage(DNSADD,(WPARAM)&Domain,(LPARAM)&IPaddr);
	CDialogEx::OnOK();
}
