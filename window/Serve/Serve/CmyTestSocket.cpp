#include "stdafx.h"
#include "CmyTestSocket.h"

extern void PrintView(CString e);

CmyTestSocket::CmyTestSocket()
{
	//CAsyncSocket::CAsyncSocket(this);
}


CmyTestSocket::~CmyTestSocket()
{

}


void  CmyTestSocket::OnReceive(int nErrorCode)
{
	TCHAR S[1024] = { 0 };
	CString mystr;
	int count = 0,rest=1026;
	while (count < 1026){
		memset(S, 0, 1024 * sizeof(TCHAR));
		count += Receive(S, rest);
		rest = rest - count;
		mystr += S;
	}
	Send(mystr, 1024);
	mystr += "12345\r\n";
	PrintView(mystr);
	this->Close();
}