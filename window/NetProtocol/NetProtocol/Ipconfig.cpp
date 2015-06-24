// Ipconfig.cpp : 实现文件
//

#include "stdafx.h"
#include "NetProtocol.h"
#include "Ipconfig.h"
#include "afxdialogex.h"


// Ipconfig 对话框

IMPLEMENT_DYNAMIC(Ipconfig, CDialogEx)

Ipconfig::Ipconfig(CWnd* pParent /*=NULL*/)
	: CDialogEx(Ipconfig::IDD, pParent)
{

}

Ipconfig::~Ipconfig()
{
}

void Ipconfig::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Ipconfig, CDialogEx)
	ON_BN_CLICKED(IDOK, &Ipconfig::OnBnClickedOk)
END_MESSAGE_MAP()


// Ipconfig 消息处理程序


void Ipconfig::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP1);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP2);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP3);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP4);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP5);
	CDialogEx::OnOK();
}
