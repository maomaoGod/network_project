#pragma once
//#include "UserCmd.h"
//#include "CmdView.h"
#include "stdafx.h"
#include "Tools.h"
using namespace Tools;

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
		CSocket aSocket;

		CString strIP;

		strIP.Format(_T("%s"), _T("172.16.162.1"));
		if (!aSocket.Create())
		{
			CString error;
			error.Format(_T("创建失败:%d"), aSocket.GetLastError());
			PrintLog(error);
			return;
		}
		//转换需要连接的端口内容类型
		PrintLog(_T("套接字创建成功"));
		
		//连接指定的地址和端口
		if (aSocket.Connect(strIP, 6500))
		{
			PrintLog(_T("连接服务器成功"));
			TCHAR szRecValue[1024] = { 0 };
			CString  sed, rev;
			int times = 0;
			TakeOverCmd(_T("clinet>: "));
			aSocket.Receive((void *)szRecValue, 1024);
			rev.Format(_T("来自服务器的消息:%s"), szRecValue);
			PrintRp(rev);
			while ((sed = GetLine()).Compare(_T("exit")) != 0){
				aSocket.Send(sed, sed.GetLength()*sizeof(TCHAR));
				aSocket.Receive((void *)szRecValue, 1024);
				rev.Format(_T("来自服务器的消息:%s"), szRecValue);
				PrintRp(rev);
			}
		}
		else{
			CString error;
			error.Format(_T("创建失败:%d"), aSocket.GetLastError());
			PrintLog(error);
		}
		aSocket.Close();
}
void Mail(CString e){
	MailSend mail;
	mail.Begin();
}