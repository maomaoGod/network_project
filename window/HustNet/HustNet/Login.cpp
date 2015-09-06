// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "HustNet.h"
#include "Login.h"
#include "afxdialogex.h"
#include "SocketClient.h"


// Login 对话框

IMPLEMENT_DYNAMIC(Login, CDialogEx)

Login::Login(CWnd* pParent /*=NULL*/)
	: CDialogEx(Login::IDD, pParent)
{
	A.Empty();
	B.Empty();
	C.Empty();
}

Login::~Login()
{
}

void Login::GetInfo(CString &ServeIP, CString &User, CString &Password)
{
	ServeIP = A;
	User = B;
	Password = C;
}

void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Login, CDialogEx)
	ON_BN_CLICKED(IDOK, &Login::OnBnClickedOk)
END_MESSAGE_MAP()


// Login 消息处理程序


void Login::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	GetDlgItem(IDC_SERVEIP)->GetWindowText(A);
	GetDlgItem(IDC_USERNAME)->GetWindowText(B);
	GetDlgItem(IDC_PASSWORD)->GetWindowText(C);
	if (A.IsEmpty() || B.IsEmpty() || C.IsEmpty()){
		AfxMessageBox(_T("信息不完整!"));
	}
	else{
		SocketClient mysock(A);
		if (mysock.Check(B, C))
			CDialogEx::OnOK();
		else{
			AfxMessageBox(_T("用户名或密码错误!"));
		}
	}
}