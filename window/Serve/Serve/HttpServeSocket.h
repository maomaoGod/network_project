#pragma once
#include "CmyAsyncSocket.h"
// HttpServeSocket ÃüÁîÄ¿±ê


class HttpServeSocket : public CmyAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


