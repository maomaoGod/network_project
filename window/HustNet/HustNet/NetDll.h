/**@file
*@brief  
*@date 2015.4.20
*@version <0.1>
*@note
*����NetDll���ͷ�ļ�
*/
#pragma once
/**
*@class <NetDll>
*@brief   ���ض�̬���ӿ���
*@note
*���ڶ�̬���ض�̬���ӿ�
*/

/**
*NetDll��Ҫʵ�ֶ�̬���ض�̬���ӿ�Ĺ��ܡ�NetDll�ṩ���ֶ�̬�������ӿ�ķ�ʽ����һ�ַ�ʽ����ͨ��·�����أ�\n
*�ڶ��ַ�ʽ����ͨ���򿪶Ի���ѡ��ָ���Ķ�̬���ӿ��ļ����м��أ����к���һ��ʵ����������������ʱʵ�����\n
*����ָ��ָ���Ķ�̬���ӿ⣻NetDll�ṩGetFun����������̬���ӿ��е�ָ�������ӿڣ�����ͨ���ӿڷ��ʶ�̬���ӿ�\n
*�еĺ�����
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
