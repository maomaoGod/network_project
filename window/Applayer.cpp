// Applayer.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Applayer.h"
#include "afxdialogex.h"


// Applayer 对话框

IMPLEMENT_DYNAMIC(Applayer, CDialogEx)

Applayer::Applayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(Applayer::IDD, pParent)
{

}

Applayer::~Applayer()
{
}

void Applayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Applayer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Applayer::OnBnClickedButton1)
END_MESSAGE_MAP()


// Applayer 消息处理程序


void Applayer::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("应用层"));
}
