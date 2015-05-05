// FTPServeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "FTPServeSocket.h"
#include "FTPClientSocket.h"
#include <map>
using namespace std;
extern void PrintView(CString);

using namespace std;
map <FTPClientSocket *, int> myclient;

// FTPServeSocket

FTPServeSocket::FTPServeSocket()
{
}

FTPServeSocket::~FTPServeSocket()
{
}


// FTPServeSocket 成员函数
void FTPServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	FTPClientSocket  *ftpClientSocket = new FTPClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*ftpClientSocket);
	Hint.Format(_T("第%d次FTP请求已连接"), count + 1);
	myclient[ftpClientSocket] = count++;
	PrintView(Hint);
}