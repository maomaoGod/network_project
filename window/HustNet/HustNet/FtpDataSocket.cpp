// FTPDataSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "FTPDataSocket.h"
#include "Tools.h"
#include "FtpTranSocket.h"
#include "map"
using namespace Tools;
using namespace std;
map <FtpTranSocket *, int> myclient;
// FTPDataSocket

FtpDataSocket::FtpDataSocket()
{
}

FtpDataSocket::~FtpDataSocket()
{
}

void FtpDataSocket::OnAccept(int nErrorCode)
{//accapt
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	FtpTranSocket  *ftpClientSocket = new FtpTranSocket();
	myclient[ftpClientSocket] = count++;
	CAsyncSocket::OnAccept(nErrorCode);
	Hint = _T("Nothing else");
	while (!good);
	good = false;
	ftpClientSocket->src = this->src_path;
	ftpClientSocket->des = this->des_path;
	ftpClientSocket->style = this->style;
	this->Accept(*ftpClientSocket);
}
