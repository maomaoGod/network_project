/**@file
*@brief  
*@date 2015.4.20
*@version <0.1>
*@note
*声明NetDll类的头文件
*/
#pragma once
/**
*@class <NetDll>
*@brief   加载动态链接库类
*@note
*用于动态加载动态链接库
*/

/**
*NetDll主要实现动态加载动态链接库的功能。NetDll提供两种动态加载链接库的方式，第一种方式可以通过路径加载；\n
*第二种方式可以通过打开对话框选择指定的动态链接库文件进行加载；类中含有一个实例句柄，当加载完成时实例句柄\n
*将被指向指定的动态链接库；NetDll提供GetFun函数导出动态链接库中的指定函数接口，程序通过接口访问动态链接库\n
*中的函数。
*/
class NetDll
{
public:
	 NetDll();
	~NetDll(); 
private:
    HINSTANCE Hinst;
public:
	void OpenDll();
	BOOL LoadDll(LPCTSTR path);
	FARPROC GetFun(LPCSTR);
};
