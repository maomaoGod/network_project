/**
*@file
*@brief HttpЭ��������ӿڵ�ʵ�ֵ�ͷ�ļ�
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
*/
#pragma once
#include "CmyAsyncSocket.h"

///<HttpServeSocket ����Ŀ��
class HttpServeSocket : public CmyAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


