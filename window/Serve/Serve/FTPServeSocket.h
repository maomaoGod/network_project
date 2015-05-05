#pragma once

// FTPServeSocket ÃüÁîÄ¿±ê

class FTPServeSocket : public CAsyncSocket
{
public:
	FTPServeSocket();
	virtual ~FTPServeSocket();
	virtual void OnAccept(int nErrorCode);
};


