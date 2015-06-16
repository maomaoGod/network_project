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
	/*CString mystr;
	int count = 0;
	while (count < 2000){
		count += Receive(S, 10000);
		mystr += S;
	}*/
	Receive(S,2000);
	Send(S, 2000);
	PrintView(S);
}