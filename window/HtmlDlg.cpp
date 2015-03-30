// HtmlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "afxinet.h"
#include "CSnet.h"
#include "NetHtml.h"
#include "HtmlDlg.h"
#include "afxdialogex.h"


IMPLEMENT_DYNAMIC(HtmlDlg, CDialogEx)

HtmlDlg::HtmlDlg(CWnd* pParent /*=NULL*/)
	:CDialogEx(HtmlDlg::IDD, pParent)
{
}

HtmlDlg::~HtmlDlg()
{
}

void HtmlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HtmlDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &HtmlDlg::OnBnClickedButton1)
END_MESSAGE_MAP()

struct ARG { LPCTSTR pstrURL; NetHtml *pHtml; CString *HtmlData; };
// HtmlDlg 消息处理程序
DWORD WINAPI GetHtml(LPVOID args)
{
	ARG *myarg = (ARG *)args;
	*(myarg->HtmlData) = (myarg->pHtml)->getURLContext(myarg->pstrURL);
	return 1;
}

void HtmlDlg::OnBnClickedButton1() //获取URL字符串，调用函数获取网页数据，并将数据输入到指定编辑框控件
{
	TCHAR url[100];
	CString myhtml;
	ARG     myarg;
	HANDLE  sonthread;
	GetDlgItem(IDC_URL)->GetWindowText(url, 100);  //获取url
	myarg.pstrURL = url;
	myarg.pHtml = &(this->Html);
	myarg.HtmlData = &myhtml;
	sonthread = CreateThread(NULL, NULL, GetHtml,(LPVOID)(&myarg),NULL,NULL);
	COleVariant noArg;
	myweb.Navigate(url, &noArg, &noArg, &noArg, &noArg);
	WaitForSingleObject(sonthread, INFINITE);
	GetDlgItem(IDC_HTML)->SetWindowText(myhtml); //显示文本
}


BOOL HtmlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rect;
	GetDlgItem(IDC_PAGE)->GetWindowRect(&rect);
	ScreenToClient(&rect);    
	GetDlgItem(IDC_PAGE)->ShowWindow(SW_HIDE);
	if (!myweb.Create(NULL, NULL, WS_VISIBLE, rect, this, 101))  
	{
		return NULL;   
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}
