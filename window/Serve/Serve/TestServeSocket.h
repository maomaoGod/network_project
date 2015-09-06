#pragma once
#include "CmyAsyncSocket.h"

class CTestServeSocket:public CAsyncSocket
{
public:
	CTestServeSocket();
	~CTestServeSocket();
	virtual void OnAccept(int nErrorCode);
};
