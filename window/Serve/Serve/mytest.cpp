// mytest.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "mytest.h"
#include "afxdialogex.h"


// mytest 对话框

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


// mytest 消息处理程序
