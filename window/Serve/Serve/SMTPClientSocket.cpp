// SMTPClientSocket.cpp : 实现文件
//

#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "SMTPClientSocket.h"

#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <SMTPClientSocket *, int> myclient;

// SMTPClientSocket

SMTPClientSocket::SMTPClientSocket()
{
}

SMTPClientSocket::~SMTPClientSocket()
{
}


SMTP app;
// SMTPClientSocket 成员函数
void SMTPClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//来自客户端%d的消息: myclient[this],
		result.Format(_T("%s"), Buffer);
		PrintView(result);
		app.CMD_Dispatch(STR::CS2S(result));
		//char *stemp;
		if (app.GetCode()==0)
		{
			Send(NULL, 0);
		}
		else
		if (app.GetCode() == OK){
			back = STR::S2CS(app.GetMsg());
			Send(back, back.GetLength()*sizeof(TCHAR));
			Send(NULL, 0);
		}
		else{
			back.Format(_T("%d"), app.GetCode());
			back = back + _T("\r\n") + STR::S2CS(app.GetMsg());
			Send(back, back.GetLength()*sizeof(TCHAR));
			Send(NULL, 0);
		}
		PrintView(back);
		//back.Format(_T("成功接收消息:%s"), Buffer);
		//Send(back, back.GetLength()*sizeof(TCHAR));
		//Send(NULL, 0);
		//Exit the Socket
		if (app.GetCode() == SMTP_QUIT)  this->Close();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
void SMTPClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("已连接服务器"));
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void SMTPClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
