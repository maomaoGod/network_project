// dll.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����:  MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  �ڴ˴�ΪӦ�ó������Ϊ��д���롣
		}
	}
	else
	{
		// TODO:  ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����:  GetModuleHandle ʧ��\n"));
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
		int nLen = iifile.GetLength();//���Ȼ�ȡ���ݳ���  
		char *pBuf = new char[nLen + 1];//�ڶ��￪��һ���ڴ�  
		pBuf[nLen] = 0;//���һ���ַ����㣬�������  
		iifile.Read(pBuf, nLen);//�����ַ� 
		sscanf_s(pBuf, "%f%f", &a, &b);
		delete[]pBuf;//ɾ������Ҫ���ڴ棬��ֹ�ڴ�й¶  
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