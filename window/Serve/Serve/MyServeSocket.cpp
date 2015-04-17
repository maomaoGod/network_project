// MyServeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "MyServeSocket.h"
#include  "ClientSocket.h"
#include <map> 

extern void PrintView(CString);

using namespace std;
map <ClientSocket *,int> myclient;



// MyServeSocket

MyServeSocket::MyServeSocket()
{
}

MyServeSocket::~MyServeSocket()
{
}


// MyServeSocket 成员函数


void MyServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	ClientSocket   *clientsocket = new ClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*clientsocket);
	Hint.Format(_T("客户端%d已连接"), count);
	myclient[clientsocket] = count++;
	PrintView(Hint);
}