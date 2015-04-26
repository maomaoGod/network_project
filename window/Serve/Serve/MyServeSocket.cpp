// MyServeSocket.cpp : 实现文件
//
/**@file
 *@brief To achieve the function of server socket
 *@author xinYao,jinxiaFang
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *XXXXXXX
 */

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
	ClientSocket  *clientsocket = new ClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*clientsocket);
	Hint.Format(_T("第%d次网页请求已连接"), count + 1);
	myclient[clientsocket] = count++;
	PrintView(Hint);
}
