#include "stdafx.h"
#include "CmyTestSocket.h"

extern void PrintView(CString e);

CmyTestSocket::CmyTestSocket()
{
	//CmyAsyncSocket::CmyAsyncSocket(this);
}


CmyTestSocket::~CmyTestSocket()
{
}


void  CmyTestSocket::OnReceive(int nErrorCode)
{
	TCHAR S[100];
	memset(S, 0, 100 * sizeof(TCHAR));
	Receive(S, 100);
	Send(S, 10);
	PrintView(S);
}