#pragma once
#include "stdafx.h"
#include "NetDll.h"
/**
*UserCmd��Ϊ�Զ�����û���������ļ��ϡ��û��ڸ���ʵ������Ĵ�����������Ե���UICtrl������Ŀ�Ĵ��ڵ�\n
*��������Լ��ṩ���������ܡ������Ļ���Ϊ���¹��̣�CmdView��Initial��ʱ����û�������䴦�����ĵ�ַ���뵽\n
*map�����У���ɳ�ʼ�����������������ʱ��������������������ؼ���ͨ��map�����õ���������Ŀ�ĵ�ַ����\n
*���ô������������뱾���ж�Ӧ�Ĵ���������������������ͨ����ģʽ�����У�Ҳ��ͨ������TakeOverCmd�ӹ�������\n
*ʵ�ֶ�������ģʽ��������ɺ󣬷���ʱ��������Զ�������Ϣ��CmdView��CmdView�л�ģʽ�ȴ��µ��������롣
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
	void UserCmd::Visit(CString url);
	void UserCmd::sendout(CString e);
 	void UserCmd::mytest(CString e);
};

