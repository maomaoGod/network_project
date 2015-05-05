#pragma once

// FTPClientSocket ÃüÁîÄ¿±ê

class FTPClientSocket : public CAsyncSocket
{
public:
	FTPClientSocket();
	virtual ~FTPClientSocket();
	TCHAR Buffer[100];
	int MAXLEN = 100;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


