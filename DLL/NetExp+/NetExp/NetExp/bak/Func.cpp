#include "stdafx.h"
#include "Func.h"


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