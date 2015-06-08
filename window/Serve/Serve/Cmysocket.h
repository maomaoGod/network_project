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
	/** Udp������
	*/
	int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);
	/** ��������
	*/
	int Receive(void* lpBuf, int nBufLen);
	/** ����TCP����
	*/
	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);
	/** ������������
	*/
	bool Listen();
	/** ����socket
	*/
	bool Create();

	bool Create(UINT nHostPort);
	/**  ��Ϣ����ʱ����
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

