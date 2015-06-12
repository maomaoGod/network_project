#pragma once

// FTPClientSocket ÃüÁîÄ¿±ê
class SMTPClientSocket : public CAsyncSocket
{
public:
	SMTPClientSocket();
	virtual ~SMTPClientSocket();
	TCHAR Buffer[1024];
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


