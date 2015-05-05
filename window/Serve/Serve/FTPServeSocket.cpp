// FTPServeSocket.cpp : ʵ���ļ�
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


// FTPServeSocket ��Ա����
void FTPServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	static int count = 0;
	CString Hint;
	FTPClientSocket  *ftpClientSocket = new FTPClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*ftpClientSocket);
	Hint.Format(_T("��%d��FTP����������"), count + 1);
	myclient[ftpClientSocket] = count++;
	PrintView(Hint);
}