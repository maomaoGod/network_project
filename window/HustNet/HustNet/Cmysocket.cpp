
#pragma once 
#include "stdafx.h"
#include "Cmysocket.h"
#include "HustNet.h"

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

void  Cmysocket::Tchar2char(LPCTSTR src, LPSTR dst)
{
	int iLength = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, src, -1, dst, iLength, NULL, NULL);
}

void  Cmysocket::char2Tchar(LPSTR src, LPCTSTR dst, int maxlen)
{
	MultiByteToWideChar(CP_ACP, 0, src, -1, (LPWSTR)dst, maxlen);
}


int Cmysocket::Send(const void* lpBuf, int nBufLen)
{
	memset(mysock.data, 0, 2048);
	memcpy(mysock.data, lpBuf, nBufLen);
	mysock.datalength = nBufLen;
	AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKSEND, (LPARAM)&mysock);
	return nBufLen;
}

/** Udp发数据
*/
int Cmysocket::SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	memcpy(mysock.data, lpBuf, nBufLen);
	Tchar2char(lpszHostAddress, mysock.dstip);
	mysock.dstport = nHostPort;
	AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKSENDTO, (LPARAM)&mysock);
	return nBufLen;
}
/** 接收数据
*/
void  Cmysocket::OnReceive(int nErrorCode)
{

}

int  Cmysocket::Receive(void* lpBuf, int nBufLen)
{
	memcpy(mysock.data, lpBuf, mysock.datalength);
	return nBufLen;
}

/** 建立TCP连接
*/
bool  Cmysocket::Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{ 
	char buf[1024];
	memset(mysock.dstip, 0, sizeof(mysock.dstip));
	Tchar2char(lpszHostAddress, mysock.dstip);
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
	mysock.srcport = nSocketPort;
	AfxGetApp()->GetMainWnd()->SendMessage(SENDTONPC, (WPARAM)SOCKBIND, (LPARAM)&mysock);
	return true;
}
