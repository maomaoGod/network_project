#pragma once

// HttpClientSocket ����Ŀ��


class HttpClientSocket : public CAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[1024];
	int MAXLEN = 1024;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


