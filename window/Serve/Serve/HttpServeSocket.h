/**
*@file
*@brief HttpЭ��������ӿڵ�ʵ�ֵ�ͷ�ļ�
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
*/
#pragma once

///<HttpServeSocket ����Ŀ��
class HttpServeSocket : public CAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


