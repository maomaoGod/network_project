#pragma once
#include "ComSocket.h"

class  CmySocket: public CComSocket
{
public:
 	  CmySocket();
	~CmySocket();
private:
	HANDLE   ReadQueue,WriteQueue;
	HANDLE   SH, CH;
	PN pCur;
	Manager   *pReadQueue, *pWriteQueue;
private:
	bool    flag;           //创建是否成功标志
	int      LastError;   //错误代号
	bool   done=true;
	char  *pReadData;
	void   DestroySock();
	unsigned int ReadDataLen,DataLen;
	enum  { READBEGIN, READMID, READFIN } readstate;
private:
	bool  InitalWriteQueue(regstruct &);
	bool  InitalReadQueue(regstruct &);
	void   RemoveRead();
	int    RcvBegin(void* lpBuf, int nBufLen);
	bool   AddToTail(HANDLE NewNode);
	bool  WaitForSockEvent(unsigned int SOCKEVENT);
public:
	bool Listen();

	int   SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress);

	int   ReceiveFrom(void* lpBuf, int nBufLen, CString& rSocketAddress, UINT& rSocketPort, int nFlags = 0);

	bool Bind(UINT nSocketPort);

	bool Create();
    
	bool Create(UINT nHostPort);

	int    Receive(void* lpBuf, int nBufLen);
	
	bool  Accept(CmySocket& rConnectedSocket);

	void  Close();

	int    Send(const void* lpBuf, int nBufLen);

	bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort);

	void  OnReceive(int nErrorCode);

	void  OnAccept(int nErrorCode);

	void  OnClose(int nErrorCode);

	void  OnSend(int nErrorCode);

	void  OnConnect(int nErrorCode);

	int    GetLastError();
};

