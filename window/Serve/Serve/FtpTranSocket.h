#pragma once
#include "Tools.h"
using namespace Tools;
// FtpTranSocket ÃüÁîÄ¿±ê

class FtpTranSocket : public CAsyncSocket
{
public:
	FtpTranSocket();
	string Msg;
	string src, des;
	bool style;
	virtual ~FtpTranSocket();
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


