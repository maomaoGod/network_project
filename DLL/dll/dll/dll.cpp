// dll.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
Func::Func()
{
}


Func::~Func()
{
}

bool Func::GetFile()
{
	TCHAR  format[100] = _T("Files (*.txt)|*.txt||");
	CFileDialog fileDlg((int)TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, format);
	if (IDOK == fileDlg.DoModal()){
		CFile iifile(fileDlg.GetFileName(), CFile::modeRead);
		int nLen = iifile.GetLength();//首先获取数据长度  
		char *pBuf = new char[nLen + 1];//在堆里开辟一块内存  
		pBuf[nLen] = 0;//最后一个字符置零，方便输出  
		iifile.Read(pBuf, nLen);//读出字符 
		sscanf_s(pBuf, "%f%f", &a, &b);
		delete[]pBuf;//删除不需要的内存，防止内存泄露  
		return true;
	}
	return false;
}
CString Func::GetAdd()
{
	CString result = _T("");
	result.Format(_T("%f"), a + b);
	return result;
}

CString Func::GetDec()
{
	CString result = _T("");
	result.Format(_T("%f"), a - b);
	return result;
}
CString Func::GetMul()
{
	CString result = _T("");
	result.Format(_T("%f"), a * b);
	return result;
}
CString Func::GetDiv()
{
	CString result = _T("");
	result.Format(_T("%f"), a / b);
	return result;
}