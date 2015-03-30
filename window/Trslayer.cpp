// Trslayer.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Trslayer.h"
#include "afxdialogex.h"


// Trslayer 对话框

IMPLEMENT_DYNAMIC(Trslayer, CDialogEx)

Trslayer::Trslayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(Trslayer::IDD, pParent)
{

}

Trslayer::~Trslayer()
{
}

void Trslayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Trslayer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Trslayer::OnBnClickedButton1)
END_MESSAGE_MAP()


// Trslayer 消息处理程序


void Trslayer::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("运输层"));
}
