#pragma once
#include "afxsock.h"


class SocketManager :
	public CAsyncSocket
{
public:
	CFile  myfile;
	CString User, Password, Cmd;
	int  filelen, curlen;
	static int  NowPort;
	enum {MINIT,MCHECKED,MUPLOAD,MPATH,MLEN,MRECEIVE} MState;
	SocketManager();
	~SocketManager();
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	void   InitCmd(TCHAR S[1024]);
	void   DealCmd(CString, CString, CString);
	bool  Check();
};

