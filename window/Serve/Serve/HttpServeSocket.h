#pragma once
#include "CmyAsyncSocket.h"
// HttpServeSocket ����Ŀ��


class HttpServeSocket : public CmyAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


