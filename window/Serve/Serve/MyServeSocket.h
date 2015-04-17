#pragma once

// MyServeSocket ÃüÁîÄ¿±ê


class MyServeSocket : public CAsyncSocket
{
public:
	MyServeSocket();
	virtual ~MyServeSocket();
	virtual void OnAccept(int nErrorCode);
};


