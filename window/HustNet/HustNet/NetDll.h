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
*@author ACM2012
*@note  NetDll��Ҫʵ�ֶ�̬���ض�̬���ӿ�Ĺ��ܡ�NetDll�ṩ���ֶ�̬�������ӿ�ķ�ʽ����һ�ַ�ʽ����ͨ��·�����أ�\n
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
	/**@brief �򿪶�̬���ӿ�*/
	void OpenDll();
	/**@brief ͨ��������ض�̬���ӿ�*/
	BOOL LoadDll(LPCTSTR path);
	/**@brief ��ȡ����ָ��*/
	FARPROC GetFun(LPCSTR);
};
