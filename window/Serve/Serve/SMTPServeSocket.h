#pragma once
#include "CmyAsyncSocket.h"
// FTPServeSocket ����Ŀ��
class SMTPServeSocket : public CAsyncSocket
{
public:
	SMTPServeSocket();
	virtual ~SMTPServeSocket();
	virtual void OnAccept(int nErrorCode);
};

