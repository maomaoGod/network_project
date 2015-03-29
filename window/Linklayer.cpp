// Linklayer.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Linklayer.h"
#include "afxdialogex.h"


// Linklayer 对话框

IMPLEMENT_DYNAMIC(Linklayer, CDialogEx)

Linklayer::Linklayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(Linklayer::IDD, pParent)
{

}

Linklayer::~Linklayer()
{
}

void Linklayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Linklayer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Linklayer::OnBnClickedButton1)
END_MESSAGE_MAP()


// Linklayer 消息处理程序


void Linklayer::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("链路层"));
}
