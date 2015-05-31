// DNS.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DNS.h"
#include "afxdialogex.h"


// CDNS 对话框

IMPLEMENT_DYNAMIC(CDNS, CDialogEx)

CDNS::CDNS(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDNS::IDD, pParent)
{

}

CDNS::~CDNS()
{
}

void CDNS::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDNS, CDialogEx)
END_MESSAGE_MAP()


// CDNS 消息处理程序
