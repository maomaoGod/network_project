#pragma once

// HttpClientSocket ÃüÁîÄ¿±ê


class HttpClientSocket : public CAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[100];
	int MAXLEN = 100;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


