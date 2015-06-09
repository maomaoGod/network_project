#pragma once
#include "ComSocket.h"

class  CmyAsyncSocket : public CComSocket
{
public:
	CmyAsyncSocket();
	~CmyAsyncSocket();
private:
	HANDLE    ReadQueue, WriteQueue;
	HANDLE    SH, CH;
	HANDLE    Pthread;
	bool state;
	Manager   *pReadQueue, *pWriteQueue;
private:
	bool    flag;          //创建是否成功标志
	int      LastError;   //错误代号
	bool   done = true;
	char  *pReadData;
	unsigned int ReadDataLen, DataLen;
private:
	bool  InitalWriteQueue(regstruct &);
	bool  InitalReadQueue(regstruct &);
	void   RemoveRead();
	bool   AddToTail(HANDLE NewNode);
	static DWORD WINAPI NewGetSockEventThread(LPVOID);
	void   GetSockEvent();
public:
	bool Listen();

	int   SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);

	int   ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);

	bool Bind(UINT nSocketPort);

	bool Create();

	bool Create(UINT nHostPort);

	int    Receive(void* lpBuf, int nBufLen);

	void  Accept(CmyAsyncSocket& rConnectedSocket);

	void  Close();

	int    Send(const void* lpBuf, int nBufLen);

	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);

	void  OnReceive(int nErrorCode);

	void  OnAccept(int nErrorCode);

	void  OnClose(int nErrorCode);

	void  OnSend(int nErrorCode);

	void  OnConnect(int nErrorCode);

	int GetLastError();
};

