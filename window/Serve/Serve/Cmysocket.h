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

	bool Bind(UINT nSocketPort);
};