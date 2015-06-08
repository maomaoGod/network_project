// DNSADDR.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DNSADDR.h"
#include "afxdialogex.h"


// CDNSADDR 对话框

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


// CDNSADDR 消息处理程序


void CDNSADDR::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	CString Domain, IPaddr;
	GetDlgItem(IDC_domain)->GetWindowText(Domain);
	GetDlgItem(IDC_IPADDRESS)->GetWindowText(IPaddr);
	AfxGetApp()->m_pMainWnd->SendMessage(DNSADDRADD,(WPARAM)&Domain,(LPARAM)&IPaddr);
	CDialogEx::OnOK();
}
