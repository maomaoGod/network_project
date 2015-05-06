#pragma once

#include "Serve.h"

class Cmysocket
{

public:
	sockstruct mysock;
public:
	/** 构造函数
	*/
	Cmysocket();
	/** 析构函数
	*/
	~Cmysocket();
	/** Tcp发数据
	*/
public:
	int Send(const void* lpBuf, int nBufLen);
	/** Udp发数据
	*/

	bool Bind(UINT nSocketPort);

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
	/**  消息来临时接收
	*/
	void  OnReceive(int nErrorCode);

	void  char2Tchar(LPSTR src, LPCTSTR dst,int maxlen);

	void  Tchar2char(LPCTSTR src, LPSTR dst);
};
