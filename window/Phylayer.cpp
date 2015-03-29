// Phylayer.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Phylayer.h"
#include "afxdialogex.h"


// Phylayer 对话框

IMPLEMENT_DYNAMIC(Phylayer, CDialogEx)

Phylayer::Phylayer(CWnd* pParent /*=NULL*/)
	: CDialogEx(Phylayer::IDD, pParent)
{

}

Phylayer::~Phylayer()
{
}

void Phylayer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Phylayer, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Phylayer::OnBnClickedButton1)
END_MESSAGE_MAP()


// Phylayer 消息处理程序


void Phylayer::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT1)->SetWindowText(_T("物理层"));
}
