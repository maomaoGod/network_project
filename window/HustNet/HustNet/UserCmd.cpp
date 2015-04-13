#pragma once
//#include "UserCmd.h"
//#include "CmdView.h"
#include "stdafx.h"
#include "Tools.h"
#include "NetWork.h"
using namespace Tools;
using namespace NetWork;

extern void PrintLog (CString);
extern void PrintRp(CString);
extern void CleanLog();
extern void CleanRp();
extern void SetRp(CString);
extern void TakeOverCmd(CString e);
extern void SetHint(CString e);
extern CString GetLine();

void Compute(CString e){
	int pos;
	CString a, b,result;
	int c1, c2;
	pos=e.Find(_T('+'));
	a.Format(_T("%s"), e.Mid(0, pos));
	b.Format(_T("%s"), e.Mid(pos));
	c1=  _wtoi(a);
	c2 = _wtoi(b);
	result.Format(_T("%d+%d=%d"), c1, c2, c1 + c2);
	PrintRp(result);
}

void Test(CString e){
	CString mystr;
	TakeOverCmd(_T("myTest>"));
	while ((mystr = GetLine()).Compare(_T("exit"))!=0)
	     PrintRp(mystr);
}

void  Connect(CString e){
		AfxSocketInit();
		//创建 CSocket 对象
		CSocket csocket;
		CString strIP;
		strIP.Format(_T("%s"), _T("127.0.0.1"));
		if (!csocket.Create())
		{
			CString error;
			error.Format(_T("创建失败:%d"), csocket.GetLastError());
			PrintLog(error);
			return;
		}
		//转换需要连接的端口内容类型
		PrintLog(_T("套接字创建成功"));
		//连接指定的地址和端口
		if (csocket.Connect(strIP, 6500))
		{
			TCHAR szRecValue[1024] = { 0 };
			CString  rev;
			//发送内容给服务器
			PrintLog(_T("连接服务器成功"));
			csocket.Send(e, e.GetLength() + 1);
			csocket.Receive((void *)szRecValue, 1024);
			rev.Format(_T("%s"), szRecValue);
		}
		else{
			CString error;
			error.Format(_T("创建失败:%d"), csocket.GetLastError());
			PrintLog(error);
		}
		csocket.Close();
}

void Mail(CString e){
	//PrintLog(_T("Wait for being finished\n"));
	MailSend mail;
	mail.Begin();
}

void Chat(CString e){
	ChatWork chat;
	chat.Begin();
}
