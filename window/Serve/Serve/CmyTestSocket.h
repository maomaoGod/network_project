#pragma once

#include "CmyAsyncSocket.h"

class CmyTestSocket :
	public CmyAsyncSocket
{
public:
	int my;
	CmyTestSocket();
	~CmyTestSocket();
	virtual void OnReceive(int nErrorCode);
};

