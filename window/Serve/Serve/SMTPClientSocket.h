#pragma once

// FTPClientSocket ����Ŀ��
class SMTPClientSocket : public CAsyncSocket
{
public:
	SMTPClientSocket();
	virtual ~SMTPClientSocket();
	TCHAR Buffer[100];
	int MAXLEN = 100;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};

