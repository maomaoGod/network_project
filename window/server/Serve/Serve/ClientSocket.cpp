// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "ClientSocket.h"


extern void PrintView(CString e);
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
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);
	}
PrintView(Buffer);
CAsyncSocket::OnReceive(nErrorCode);
}


void ClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	Send(_T("成功接收"), sizeof(_T("成功接收")));
	CAsyncSocket::OnSend(nErrorCode);
}