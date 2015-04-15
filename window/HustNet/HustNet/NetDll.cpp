#include "stdafx.h"
#include "NetDll.h"

extern void PrintLog(CString);
NetDll::NetDll()
{
	Hinst = NULL;
}


NetDll::~NetDll()
{
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
}

BOOL NetDll:: LoadDll(LPCTSTR path){
	if (Hinst != NULL){   //释放已有句柄
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	Hinst = LoadLibrary(path); //获取链接库实例句柄
	if (Hinst == NULL){
		PrintLog(_T("加载动态链接库失败"));
		return  FALSE;
	}
	PrintLog(_T("加载动态链接库成功"));
	return TRUE;
}

FARPROC NetDll::GetFun(	LPCSTR name){
	return GetProcAddress(Hinst, name);
}

void NetDll::OpenDll(){
	TCHAR  format[100] = _T("Files (*.dll)|*.dll||");
	CFileDialog file((int)TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, format);//打开文件对话框
	file.DoModal();
	CString path;
	path = file.GetPathName();//获取链接库路径
	if (path.IsEmpty())
		return;
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	Hinst = LoadLibrary(path); //获取链接库实例句柄
	if (Hinst == NULL){
		PrintLog(_T("加载动态链接库失败!"));
		return;
	}
	PrintLog(_T("加载动态链接库失败!"));
}