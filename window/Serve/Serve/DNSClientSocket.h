#pragma once
#include "DNSServeSocket.h"
// DNSClientSocket ÃüÁîÄ¿±ê

class DNSClientSocket : public CAsyncSocket
{
public:
	DNSClientSocket();
	virtual ~DNSClientSocket();
	TCHAR Buffer[1024];
	int MAXLEN = 1024;
	dnsmap *dnshost;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


