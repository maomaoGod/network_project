#pragma once

#include "CmyAsyncSocket.h"

class CmyTestSocket :
	public CAsyncSocket
{
public:
	int my;
	CmyTestSocket();
	~CmyTestSocket();
	virtual void OnReceive(int nErrorCode);
};

