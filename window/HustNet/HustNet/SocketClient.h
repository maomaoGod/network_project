#pragma once
#include "afxsock.h"

class SocketClient :
	public CSocket
{
public:
	CString User, Password;
	bool flag;
	SocketClient();
	SocketClient(CString);
	~SocketClient();
	int   ConnectServe();
	bool Check(CString, CString);
	bool UploadFile(CFile &myfile);
};