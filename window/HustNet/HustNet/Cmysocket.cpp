
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

int Send(const void* lpBuf, int nBufLen)
{
	 sockstruct mysock;
	memcpy(mysock.data, lpBuf, nBufLen);
	return nBufLen;
}
/** Udp������
*/
int SendTo(const void* lpBuf, int nBufLen, UINT nHostPort, LPCTSTR lpszHostAddress)
{
	struct sockstruct mysock;
	memcpy(mysock.data, lpBuf, nBufLen);
	memcpy(mysock.dstip, lpszHostAddress, sizeof(lpszHostAddress));
	mysock.dstport = nHostPort;
	return nBufLen;
}
/** ��������
*/
int    Receive(void* lpBuf, int nBufLen)
{
	struct sockstruct mysock;
	 memcpy(mysock.data, lpBuf, nBufLen);
	 return nBufLen;
}

/** ����TCP����
*/
bool  Connect(LPCTSTR lpszHostAddress, UINT nHostPort)
{
	return true;
}
/** ������������
*/
bool Listen()
{
	return true;
}
/** ����socket
*/
bool Create()
{
	return true;
}
