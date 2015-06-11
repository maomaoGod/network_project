#pragma once
#include "CmyAsyncSocket.h"

class CTestServeSocket:public CmyAsyncSocket
{
public:
	CTestServeSocket();
	~CTestServeSocket();
	virtual void OnAccept(int nErrorCode);
};
