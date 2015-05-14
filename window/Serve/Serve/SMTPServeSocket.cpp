// SMTPServeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "SMTPServeSocket.h"
#include "SMTPClientSocket.h"
#include <map>
using namespace std;
extern void PrintView(CString);

using namespace std;
map <SMTPClientSocket *, int> myclient;

// SMTPServeSocket

SMTPServeSocket::SMTPServeSocket()
{
}

SMTPServeSocket::~SMTPServeSocket()
{
}


// SMTPServeSocket 成员函数
void SMTPServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	SMTPClientSocket  *smtpClientSocket = new SMTPClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*smtpClientSocket);
	Hint.Format(_T("第%d次SMTP请求已连接"), count + 1);
	myclient[smtpClientSocket] = count++;
	PrintView(Hint);
}