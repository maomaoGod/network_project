// SMTPServeSocket.cpp : ʵ���ļ�
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


// SMTPServeSocket ��Ա����
void SMTPServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	static int count = 0;
	CString Hint;
	SMTPClientSocket  *smtpClientSocket = new SMTPClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*smtpClientSocket);
	Hint.Format(_T("��%d��SMTP����������"), count + 1);
	myclient[smtpClientSocket] = count++;
	PrintView(Hint);
}