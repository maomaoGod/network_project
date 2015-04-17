// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "ClientSocket.h"
#include <map>

using namespace std;
extern void PrintView(CString e);
extern map <ClientSocket *, int> myclient;
// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 成员函数

void ClientSocket::OnReceive(int nErrorCode)
{
// TODO:  在此添加专用代码和/或调用基类
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);
		result.Format(_T("来自客户端%d的消息:%s"), myclient[this], Buffer);
		PrintView(result);
		back.Format(_T("成功接收消息:%s"), Buffer);
		Send(back, back.GetLength()*sizeof(TCHAR));
	}
CAsyncSocket::OnReceive(nErrorCode);
}


void ClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("今天第%d位登录者"), myclient[this]);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void ClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
