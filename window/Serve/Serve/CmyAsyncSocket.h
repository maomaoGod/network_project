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
	bool state = true;
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
	bool  WaitForSockEvent(unsigned int SOCKEVENT);
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

	bool  Accept(CmyAsyncSocket& rConnectedSocket);

	void  Close();

	int    Send(const void* lpBuf, int nBufLen);

	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);

	virtual void  OnReceive(int nErrorCode);

	virtual void  OnAccept(int nErrorCode);

	virtual  void  OnClose(int nErrorCode);

	virtual void  OnSend(int nErrorCode);

	virtual void  OnConnect(int nErrorCode);

	int GetLastError();
};

