#pragma once

// HttpServeSocket ÃüÁîÄ¿±ê


class HttpServeSocket : public CAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


