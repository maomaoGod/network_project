#pragma once
#include "CmyAsyncSocket.h"
// FTPServeSocket ÃüÁîÄ¿±ê
class SMTPServeSocket : public CAsyncSocket
{
public:
	SMTPServeSocket();
	virtual ~SMTPServeSocket();
	virtual void OnAccept(int nErrorCode);
};

