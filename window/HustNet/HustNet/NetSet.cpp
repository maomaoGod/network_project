// NetSet.cpp : 实现文件
//

#include "stdafx.h"
#include "HustNet.h"
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
	ON_BN_CLICKED(IDC_LOCAL_Browse, &NetSet::OnBnClickedLocalBrowse)
	ON_BN_CLICKED(IDC_Server_Browse, &NetSet::OnBnClickedServerBrowse)
END_MESSAGE_MAP()


// NetSet 消息处理程序


void NetSet::OnBnClickedLocalBrowse()
{
	// TODO:  在此添加控件通知处理程序代码
	CString local_path;
	CFileDialog dlg(TRUE,//t open f save
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_T("DLL Files (*.dll)|*.dll"),
		NULL);
	if (dlg.DoModal() == IDOK){
		local_path = dlg.GetPathName();
		GetDlgItem(IDC_local_text)->SetWindowText(local_path);
	}
	else return;
}


void NetSet::OnBnClickedServerBrowse()
{
	// TODO:  在此添加控件通知处理程序代码
	CString server_path;
	CFileDialog dlg(TRUE,//t open f save
		NULL, NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_T("DLL Files (*.dll)|*.dll"),
		NULL);
	if (dlg.DoModal() == IDOK){
		server_path = dlg.GetPathName();
		GetDlgItem(IDC_server_text)->SetWindowText(server_path);
	}
	else return;
}
