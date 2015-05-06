/**@file
*@brief  输入输出API
*@date 2015.4.20
*@version <0.1>
*@note
*界面控制的实现
*/
#pragma once
#include  "stdafx.h"
#include  "HustNet.h"
#include  "UICtrl.h"
#include  "MainFrm.h"
#include  "CmdView.h"

CMainFrame *pframe;
/**
*打印一行文字到日志窗口\n
*函数发送PRINT消息到LogView窗口，并将字符串封装到lparam中，LogView窗口接收到该消息后打印相应字符串到LogView窗口\n
* @param log 需要打印的日志字符串
*/
void PrintLog(CString log)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.para1 = &log;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, LOGVIEW, (LPARAM)(&mycmd));
}


/**
*打印一行文字到响应窗口\n
*函数发送PRINT消息到RespondView窗口，并将字符串封装到lparam中，RespondView窗口接\n
*收到该消息后打印相应字符串到RespondView窗口\n
* @param rp 需要打印的字符串
*/
void PrintRp(CString rp)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.para1 = &rp;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, (LPARAM)(&mycmd));
}

/**
*清空日志窗口\n
*函数发送CLEAN消息到LogView窗口，LogView窗口接收到该消息后打印相应字符串到LogView窗口
*/
void CleanLog(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, LOGVIEW, LPARAM(&mycmd));
}

/**
*清空响应窗口
*函数发送CLEAN消息到LogView窗口，RespondView窗口接收到该消息后打印相应字符串到LogView窗口
*/
void CleanRp(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}


/**
*设置相应窗口的内容\n
*函数发送SETTEXT消息到RespondView窗口，并将字符串封装到lparam中，RespondView窗口接\n
*收到该消息后打印相应字符串到RespondView窗口\n
* @param e 需要设置的字符串
*/
void SetRp(CString e)
{
	CMD mycmd;
	mycmd.ID = SETTEXT;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}

/**
*设置CmdView的命令提示语\n
*函数发送SETHINT消息到CmdView窗口，并将字符串封装到lparam中，CmdView窗口接\n
*收到该消息后设置HINT命令提示语\n
* @param e 需要设置的命令提示语
*/
void SetHint(CString e)
{
	CMD mycmd;
	mycmd.ID = SETHINT;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}


/**
*设置CmdView的命令提示语，接管CmdView的输入\n
*函数发送TAKEOVERCMD消息到CmdView窗口，并将字符串封装到lparam中，CmdView窗口接\n
*收到该消息后设置HINT命令提示语，并将CmdView的状态切换到用户命令模式，从而用户命令可以\n
*从CmdView中获取数据\n
*param e 需要设置的命令提示语
*/
void TakeOverCmd(CString e)
{
	CMD mycmd;
	mycmd.ID = TAKEOVERCMD;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

/**
*从CmdView中获取一行输入\n
*函数在用户命令模式下从Cmd获取一行输入。函数首先判断当前是否处于用户命令模式\n
* @retval null 不在用户命令模式
*函数采用循环测试方式，当CmdView获取新的一行输入，获取该行并返回*/
CString  GetLine()
{
	static CString  myline;
	CMDMODE cmdflag;
	LINESTATE lineflag;
	CMD mycmd;
	mycmd.ID = GETCMDSTATE;
	mycmd.para1 = &cmdflag;
	mycmd.para2 = NULL;
	if (pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd)) &&cmdflag!=USER){
		PrintLog(_T("未接管CmdView窗口"));
		return NULL;
	}
	lineflag = OLDLINE;
	mycmd.ID = GETLINESTATE;
	mycmd.para1 = &lineflag;
	while (!pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd)) && lineflag == OLDLINE)
		;
	ASSERT(lineflag == NEWLINE);
	mycmd.ID = GETCOMMAND;
	mycmd.para1 = &myline;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
	EndInput();
	return myline;
}

/**
*通知主窗口命令已获取完成\n
*函数发送ENDINPUT消息到CmdView，通知用户模式下成功获取了一行输入。CmdView接收到该命令后，\n
*会重新输出命令提示语，等待下一行子命令的输入*/
void EndInput()
{
	CMD mycmd;
	mycmd.ID = ENDINPUT;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

void SendOut(CString e)
{
	pframe->SendMessage(SENDTONPC, (WPARAM)&e);
}

