#pragma once

#include "stdafx.h"
#include "Serve.h"

class CmySocket
{
public:
   	  CmySocket();
	~CmySocket();
public:
	ObjEvent    myEvent;
	bool flag;
	int SockMark;
public:
	bool InitalEvent(regstruct *myreg);
	int   Send(const void* lpBuf, int nBufLen);
	/** Udp发数据
	*/
	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	/** 接收数据
	*/
	int Receive(void* lpBuf, int nBufLen);
	/** 建立TCP连接
	*/
	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	/** 监听连接请求
	*/
	bool Listen();
	/** 创建socket
	*/
	bool Create();

	bool Create(UINT nHostPort);
	/**  消息来临时接收
	*/
	void  OnReceive(int nErrorCode);

	void  OnAccept(int nErrorCode);

	void  Accept(CmySocket & rConnectedSocket);

	bool Bind(UINT nSocketPort);

	void  char2Tchar( LPSTR src, LPCTSTR dst, int maxlen);

	void  Tchar2char(LPCTSTR src, LPSTR dst);

	void  Close();

	void GetSockMark(regstruct *preg, regstruct *myreg);
};

