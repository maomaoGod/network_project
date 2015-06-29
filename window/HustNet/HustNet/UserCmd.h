#pragma once
#include "stdafx.h"
#include "NetDll.h"

/**
* @class <UserCmd>
* @brief   UserCmd��Ϊ�Զ�����û���������ļ���
* @author ACM2012
* @note UserCmd��Ϊ�Զ�����û���������ļ��ϡ��û��ڸ���ʵ������Ĵ�����������Ե���UICtrl
* ������Ŀ�Ĵ��ڵ���������Լ��ṩ���������ܡ������Ļ���Ϊ���¹��̣�CmdView��Initial��ʱ����û�
* ������䴦�����ĵ�ַ���뵽\nmap�����У���ɳ�ʼ�����������������ʱ��������������������ؼ�
* ��ͨ��map�����õ���������Ŀ�ĵ�ַ�������ô������������뱾���ж�Ӧ�Ĵ���������������������
* ͨ����ģʽ�����У�Ҳ��ͨ������TakeOverCmd�ӹ�������ʵ�ֶ�������ģʽ��������ɺ󣬷���ʱ���������
* ��������Ϣ��CmdView��CmdView�л�ģʽ�ȴ��µ���������
*/
class UserCmd
{
public:
	UserCmd();
	~UserCmd();
	NetDll mydll;
	/**
	*��������ʾ��
	*/
	void UserCmd::cleanrp(CString e);
	void UserCmd::cleanlog(CString e);
	void UserCmd::Compute(CString);
	void UserCmd::Test(CString);
	void UserCmd::Connect(CString e);
	void UserCmd::Http(CString);
	void UserCmd::Chat(CString e);
	void UserCmd::SetDll(CString e);
	void UserCmd::GetHtml(CString url);
	//void UserCmd::Visit(CString url);
	void UserCmd::Ftp(CString url);
	void UserCmd::mytest(CString e);
};

