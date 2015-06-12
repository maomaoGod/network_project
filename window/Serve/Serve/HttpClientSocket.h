#pragma once

// HttpClientSocket ����Ŀ��
#include "Tools.h"

using namespace Tools;

class HttpClientSocket : public CAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[BUFSIZE];
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


