// NetSet.cpp : 实现文件
//
#include "stdafx.h"
#include "ACMNet.h"
#include "NetSet.h"
#include "afxdialogex.h"

// NetSet 对话框

IMPLEMENT_DYNAMIC(NetSet, CDialogEx)

NetSet::NetSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(NetSet::IDD, pParent)
{

}

NetSet::~NetSet()
{
}

void NetSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(NetSet, CDialogEx)
END_MESSAGE_MAP()

