#pragma once

// FTPClientSocket ÃüÁîÄ¿±ê

#include "Ftpworker.h"
#include "FTPDataSocket.h"

class FTPClientSocket : public CAsyncSocket
{
public:
	FTPClientSocket();
	virtual ~FTPClientSocket();
	TCHAR Buffer[BUFSIZE];
	Ftpworker ftpworker;
	FTPDataSocket *dataSocket;
	CSocket *dataSend;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


