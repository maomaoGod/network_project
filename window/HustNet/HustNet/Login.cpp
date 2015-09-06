// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HustNet.h"
#include "Login.h"
#include "afxdialogex.h"
#include "SocketClient.h"


// Login �Ի���

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


// Login ��Ϣ�������


void Login::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	GetDlgItem(IDC_SERVEIP)->GetWindowText(A);
	GetDlgItem(IDC_USERNAME)->GetWindowText(B);
	GetDlgItem(IDC_PASSWORD)->GetWindowText(C);
	if (A.IsEmpty() || B.IsEmpty() || C.IsEmpty()){
		AfxMessageBox(_T("��Ϣ������!"));
	}
	else{
		SocketClient mysock(A);
		if (mysock.Check(B, C))
			CDialogEx::OnOK();
		else{
			AfxMessageBox(_T("�û������������!"));
		}
	}
}