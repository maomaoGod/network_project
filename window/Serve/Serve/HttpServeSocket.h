#pragma once

// HttpServeSocket ����Ŀ��


class HttpServeSocket : public CAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


