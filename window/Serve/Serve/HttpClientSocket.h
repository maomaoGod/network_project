/**
*@file
*@brief HttpЭ��ͻ��˽ӿڵ�ʵ�ֵ�ͷ�ļ�
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
*/
#pragma once


#include "Tools.h"
#include "CmyAsyncSocket.h"

using namespace Tools;
///<HttpClientSocket ����Ŀ��
class HttpClientSocket : public CmyAsyncSocket
{
public:
	HttpClientSocket();
	virtual ~HttpClientSocket();
	TCHAR Buffer[BUFSIZE];
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


