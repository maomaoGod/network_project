#pragma once
#include "Tools.h"
using namespace Tools;
// FTPDataSocket ÃüÁîÄ¿±ê

class FTPDataSocket : public CAsyncSocket
{
public:
	FTPDataSocket();
	TCHAR Buffer[1024];
	bool style, finish, good;
	string src_path, des_path;
	virtual ~FTPDataSocket();
	virtual void OnAccept(int nErrorCode);
};


