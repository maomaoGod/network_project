#pragma once
#include "afxsock.h"
#include "SocketManager.h"
class SocketListen :
	public CAsyncSocket
{
public:
	SocketListen();
	~SocketListen();
	virtual void OnConnect(int nErrorCode);
	virtual void OnAccept(int nErrorCode);
};

