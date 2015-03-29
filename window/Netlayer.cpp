// Netlayer.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Netlayer.h"
#include "afxdialogex.h"


// Netlayer 对话框

IMPLEMENT_DYNAMIC(Netlayer, CDialogEx)

Netlayer::Netlayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(Netlayer::IDD, pParent)
{

}

Netlayer::~Netlayer()
{
}

void Netlayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Netlayer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Netlayer::OnBnClickedButton1)
END_MESSAGE_MAP()


// Netlayer 消息处理程序


void Netlayer::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("网络层"));
}
