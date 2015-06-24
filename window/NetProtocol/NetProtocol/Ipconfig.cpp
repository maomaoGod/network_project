// Ipconfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NetProtocol.h"
#include "Ipconfig.h"
#include "afxdialogex.h"


// Ipconfig �Ի���

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


// Ipconfig ��Ϣ�������


void Ipconfig::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP1);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP2);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP3);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP4);
	GetDlgItem(IDC_IPADDRESS1)->GetWindowText(IP5);
	CDialogEx::OnOK();
}
