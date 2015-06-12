#pragma once
#include "Tools.h"
using namespace Tools;
// FtpDataSocket ÃüÁîÄ¿±ê

class FtpDataSocket : public CAsyncSocket
{
public:
	FtpDataSocket();
	virtual ~FtpDataSocket();
	TCHAR Buffer[1024];
	bool style, good;
	string src_path, des_path;
	virtual void OnAccept(int nErrorCode);
};


