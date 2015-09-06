/**@file
*@brief
*@author 李贤亮 刘永博
*@date 2015.4.20
*@version <0.1>
*@note
*声明NetDll类的头文件
*/
#include "stdafx.h"
#include "NetDll.h"

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

/**
 *通过路径加载动态链接库
 *函数实现通过路径加载动态链接库。函数首先释放已有句柄（若存在），然后通路路径加载动态链接库
 * @param path  动态链接库的字符串路径
 * @retval TRUE 成功加载
 * @retval FALSE 失败
 */
BOOL NetDll:: LoadDll(LPCTSTR path){
	/**
	*释放已有句柄
	*/
	if (Hinst != NULL){   //释放已有句柄
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	/**
	*获取链接库实例句柄
	*/
	Hinst = LoadLibrary(path); //获取链接库实例句柄
	if (Hinst == NULL){
		//PrintLog(_T("加载动态链接库失败"));
		return  FALSE;
	}
	//PrintLog(_T("加载动态链接库成功"));
	return TRUE;
}

/**
*函数通过函数名和以保存的动态链接库句柄返回函数地址
* @param name 所调用函数的函数名
*/

FARPROC NetDll::GetFun(	LPCSTR name){
	if (Hinst == NULL)
		return NULL;
	return GetProcAddress(Hinst, name);
}

void NetDll::FreeDll(){
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	return;
}

/**
*通过文件对话框加载动态链接库\n
*函数通过打开文件对话框加载动态链接库。函数激活一个文件对话框窗口，\n
*获取选中文件的路径，然后通过所获取的文件路径加载动态链接库
*/
void NetDll::OpenDll(){
	TCHAR  format[100] = _T("Files (*.dll)|*.dll||");
	/**
	*打开文件对话框
	*/
	CFileDialog file((int)TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, format);//打开文件对话框
	file.DoModal();
	CString path;
	/**
	*获取链接库路径
	*/
	path = file.GetPathName();//获取链接库路径
	if (path.IsEmpty())
		return;
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	/**
	*获取链接库实例句柄
	*/
	Hinst = LoadLibrary(path); //获取链接库实例句柄
	if (Hinst == NULL){
		//PrintLog(_T("加载动态链接库失败!"));
		return;
	}
	//PrintLog(_T("加载动态链接库成功!"));
}

