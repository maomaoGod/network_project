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
	TCHAR S[10000] = { 0 };
	Receive(S, 2000);
	Send(S, 2000);
	PrintView(S);
}