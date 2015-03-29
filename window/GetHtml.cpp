// GetHtml.cpp : 实现文件
//

#include "stdafx.h"
#include "afxinet.h"
#include "CSnet.h"
#include "GetHtml.h"
#include "afxdialogex.h"

CString  GetHtml::getURLContext(LPCTSTR pstrURL){
	CString myhtml;	
	/*CInternetSession mySession(NULL, 0);
	CHttpFile* htmlFile = NULL;
	TCHAR src[1024];
	try{
		htmlFile = (CHttpFile*)mySession.OpenURL(pstrURL);//建立连接获取输入流
		if (htmlFile != NULL){//显示网页
			COleVariant noArg;
			myweb.Navigate(pstrURL, &noArg, &noArg, &noArg, &noArg);
		}
		while (htmlFile->ReadString(src, 1024)){//处理流中每一行
			int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0); 
			TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
			memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8转Unicode
			myhtml += pBuffer; 
			myhtml += _T("\r\n");
			delete pBuffer;
		}
		htmlFile->Close();
		mySession.Close(); //关闭会话
	}
	catch (CException *e) {  //异常处理
	       AfxMessageBox(_T("获取数据失败"));
		  } */
	return myhtml; 
};

IMPLEMENT_DYNAMIC(GetHtml, CDialogEx)

GetHtml::GetHtml(CWnd* pParent /*=NULL*/)
	: CDialogEx(GetHtml::IDD, pParent)
{

}

GetHtml::~GetHtml()
{
}

void GetHtml::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GetHtml, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &GetHtml::OnBnClickedButton1)
END_MESSAGE_MAP()


// GetHtml 消息处理程序


void GetHtml::OnBnClickedButton1() //获取URL字符串，调用函数获取网页数据，并将数据输入到指定编辑框控件
{
	TCHAR url[100];
	CString myhtml;
	GetDlgItem(IDC_URL)->GetWindowText(url, 100);  //获取url		
	myhtml = getURLContext(url);        //获取数据流
	GetDlgItem(IDC_HTML)->SetWindowText(myhtml); //显示文本
}


BOOL GetHtml::OnInitDialog()
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
	// 异常:  OCX 属性页应返回 FALSE
}
