#pragma once
#include "CmyAsyncSocket.h"
// HttpClientSocket ÃüÁîÄ¿±ê


class HttpClientSocket : public CmyAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[100];
	int MAXLEN = 100;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


