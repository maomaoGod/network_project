
#pragma once 
#include "stdafx.h"
#include "Cmysocket.h"
#include "Serve.h"

Cmysocket::Cmysocket()
{

}


Cmysocket::~Cmysocket()
{

}

/*
struct sockstruct {
unsigned short  dstport;
unsigned short  bindport;
int    datalength;
char dstip[20];
char data[2048];
};

*/

int Cmysocket::Send(const void* lpBuf, int nBufLen)
{
	sockstruct mysock;
	memcpy(mysock.data, lpBuf, nBufLen);
	return nBufLen;
}
/** Udp发数据
*/
int Cmysocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	memcpy(mysock.data, lpBuf, nBufLen);
	memcpy(mysock.dstip, lpszHostAddress, sizeof(lpszHostAddress));
	mysock.dstport = nHostPort;
	return nBufLen;
}
/** 接收数据
*/
int    Cmysocket::Receive(void* lpBuf, int nBufLen)
{
	 memcpy(mysock.data, lpBuf, nBufLen);
	 return nBufLen;
}

/** 建立TCP连接
*/
bool  Cmysocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{ 
	char buf[1024];
	int iLength = WideCharToMultiByte(CP_ACP, 0, lpszHostAddress, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, lpszHostAddress, -1,buf, iLength, NULL, NULL);
	memcpy(mysock.dstip, buf, iLength);
	mysock.dstport = nHostPort;
    AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKCONNECT, (LPARAM)&mysock);
	return true;
}
/** 监听连接请求
*/
bool Cmysocket::Listen()
{
	AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKLISTEN, (LPARAM)&mysock);
	return true;
}
/** 创建socket
*/
bool Cmysocket::Create()
{
	return true;
}
bool Cmysocket::Bind(UINT nSocketPort)
{
	mysock.bindport = nSocketPort;
	AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKBIND, (LPARAM)&mysock);
	return true;
}
