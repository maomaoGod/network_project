#include "stdafx.h"
#include "TestServeSocket.h"
#include "CmyTestSocket.h"


CTestServeSocket::CTestServeSocket()
{

}


CTestServeSocket::~CTestServeSocket()
{

}

void CTestServeSocket::OnAccept(int nErrorCode)
{
	CString Hint;
	CmyTestSocket  *ClientSocket = new CmyTestSocket();
	this->Accept(*ClientSocket);
}