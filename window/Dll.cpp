// Dll.cpp : 实现文件
//

#include "stdafx.h"
#include "CSnet.h"
#include "Dll.h"
#include "afxdialogex.h"


// Dll 对话框

IMPLEMENT_DYNAMIC(Dll, CDialogEx)

Dll::Dll(CWnd* pParent /*=NULL*/)
	: CDialogEx(Dll::IDD, pParent)
{
	Add = NULL;

}

Dll::~Dll()
{
}

void Dll::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Dll, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Dll::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_LOAD, &Dll::OnBnClickedLoad)
END_MESSAGE_MAP()


// Dll 消息处理程序


void Dll::OnBnClickedButton1()
{
	// TODO:  触发获取输入参数，执行动态链接库函数，结果写回控件
	/*int num1, num2;
	TCHAR ch1[10], ch2[10], rst[10];
	CString result;
	GetDlgItem(IDC_NUMBER1)->GetWindowText(ch1,10);
	GetDlgItem(IDC_NUMBER2)->GetWindowText(ch2,10);
	num1 = _ttoi(ch1);
	num2 = _ttoi(ch2);
	if (Add == NULL)
		return;
	result.Format(_T("%d + %d = %d"), num1, num2, Add(num1, num2));
	GetDlgItem(IDC_RESULT)->SetWindowText(result);*/
}


void Dll::OnBnClickedLoad()
{
	// TODO:  在此加载动态链接库，将Dll类中的函数指针指向该动态链接库导出函数(PS:示例为Add)
	/*HINSTANCE Hinst;
	TCHAR  format[100] = _T("Files (*.dll)|*.dll||");
	CFileDialog file((int)TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY,format);//打开文件对话框
	file.DoModal(); 
	CString path;
	path = file.GetPathName();//获取链接库路径
	Hinst = LoadLibrary(path); //获取链接库实例句柄
	if (Hinst == NULL)
		return;
	Add = (ADDPROC)GetProcAddress(Hinst, "add");*/   //获取链接库导出函数指针
}
