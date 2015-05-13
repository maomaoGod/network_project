#pragma once
#include "stdafx.h"
#include "NetDll.h"
/**
*UserCmd类为自定义的用户命令处理函数的集合。用户在该类实现命令的处理，命令函数可以调用UICtrl来调用目的窗口的\n
*输入输出以及提供的其他功能。命令处理的机制为若下过程，CmdView在Initial的时候见用户命令和其处理函数的地址插入到\n
*map容器中，完成初始化。当命令输入完成时，任务分配程序会根据命令关键字通过map容器得到处理函数的目的地址，并\n
*调用处理函数，即进入本类中对应的处理函数。处理函数可以在普通命令模式下运行，也可通过调用TakeOverCmd接管命令行\n
*实现二级命令模式。命令完成后，返回时，程序会自动发送消息给CmdView，CmdView切换模式等待新的命令输入。
*/
class UserCmd
{
public:
	UserCmd();
	~UserCmd();
	NetDll mydll;
	/**
	*部分命令示例
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

