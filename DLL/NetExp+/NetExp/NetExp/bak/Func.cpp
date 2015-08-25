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