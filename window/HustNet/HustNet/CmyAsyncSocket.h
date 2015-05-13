#pragma once
#include "HustNet.h"

class CmyAsyncSocket
{
public:
	ObjEvent    myEvent;
	bool flag;
public:
	/** 构造函数
	*/
	CmyAsyncSocket();
	/** 析构函数
	*/
	~CmyAsyncSocket();
	/** Tcp发数据
	*/
public:

	bool InitalEvent(regstruct *myreg);
	int Send(const void* lpBuf, int nBufLen);
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

	void  OnClose(int nErrorCode);

	void Close();

	void  Accept(CmyAsyncSocket& rConnectedSocket);

	bool Bind(UINT nSocketPort);

	void  char2Tchar(LPSTR src, LPCTSTR dst,int maxlen);

	void  Tchar2char(LPCTSTR src, LPSTR dst);

	void GetSockMark(regstruct *preg, regstruct *myreg);

	static DWORD WINAPI ReadSock(LPVOID lParam);

	static DWORD WINAPI WriteSock(LPVOID lParam);
};
