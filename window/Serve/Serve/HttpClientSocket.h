/**
*@file
*@brief Http协议客户端接口的实现的头文件
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
*/
#pragma once


#include "Tools.h"
#include "NetDll.h"
#include "CmyAsyncSocket.h"
using namespace Tools;

typedef void *(*HttPtr)(void);
typedef void(*HttpMake)(void *);
typedef int(*HttpSetMsg)(void *, string);
typedef string(*Httpgetdata)(void*);
typedef int(*Httpanaly)(void*);
typedef string(*HttpgetMsg)(void*);
typedef string(*Httpgethost)(void*);
typedef void(*Httpcheck)(void*);
typedef int(*HttpgetPort)(void*);
typedef string(*Httplook_msg)(void*);
typedef string(*Httplook_rmsg)(void*);
typedef CString(*Httpshow_rmsg)(void*);
typedef void(*Httpsetdata)(void*, string);
///<HttpClientSocket 命令目标
class HttpClientSocket : public CmyAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[BUFSIZE];
	NetDll *dll;
	HttPtr httptr;
	HttpMake httpmake;
	HttpgetPort httpgetPort;
	Httpgethost httpgethost;
	HttpgetMsg httpgetMsg;
	HttpSetMsg httpsetmsg;
	Httpgetdata httpgetdata;
	Httpsetdata httpsetdata;
	Httpanaly httpanaly;
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


