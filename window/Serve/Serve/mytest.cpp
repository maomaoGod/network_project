// mytest.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Serve.h"
#include "mytest.h"
#include "afxdialogex.h"


// mytest �Ի���

IMPLEMENT_DYNAMIC(mytest, CDialogEx)

mytest::mytest(CWnd* pParent /*=NULL*/)
	: CDialogEx(mytest::IDD, pParent)
{

}

mytest::~mytest()
{
}

void mytest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(mytest, CDialogEx)
END_MESSAGE_MAP()


// mytest ��Ϣ�������
