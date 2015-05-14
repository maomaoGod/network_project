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
	Receive(S, 100);
	Send(S, 10);
	PrintView(S);
}