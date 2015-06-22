#pragma once

// HttpClientSocket ÃüÁîÄ¿±ê
#include "Tools.h"
#include "CmyAsyncSocket.h"

using namespace Tools;

class HttpClientSocket : public CmyAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[BUFSIZE];
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


