#pragma once

// ClientSocket ÃüÁîÄ¿±ê

class ClientSocket : public CAsyncSocket
{
public:
	ClientSocket();
	virtual ~ClientSocket();
	TCHAR Buffer[100];
	int MAXLEN = 100;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


