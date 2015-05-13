// FTPClientSocket.cpp : 实现文件
//

#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "FTPClientSocket.h"

#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <FTPClientSocket *, int> myclient;

// FTPClientSocket

FTPClientSocket::FTPClientSocket()
{
}

FTPClientSocket::~FTPClientSocket()
{
}


AppLayerFtp app;
// FTPClientSocket 成员函数
void FTPClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	vector<string> path;
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//来自客户端%d的消息: myclient[this],
		result.Format(_T("%s"), Buffer);
		PrintView(result);
		app.GetData(STR::CS2S(result));
		app.DealWith(app.GetCMD());
		//char *stemp;
		back.Format(_T("%d"), app.GetCode());
		back = back + _T("\r\n") + STR::S2CS(app.GetResMsg());
		PrintView(back);
		//back.Format(_T("成功接收消息:%s"), Buffer);
		Send(back, back.GetLength()*sizeof(TCHAR));
		Send(NULL, 0);
		//Exit the Socket
		if (app.GetCode() == QUIT_EXIT)  this->Close();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void FTPClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("已通过匿名连接服务器"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void FTPClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
