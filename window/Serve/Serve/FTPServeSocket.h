#pragma once

// FTPServeSocket ����Ŀ��

class FTPServeSocket : public CAsyncSocket
{
public:
	FTPServeSocket();
	virtual ~FTPServeSocket();
	virtual void OnAccept(int nErrorCode);
};


