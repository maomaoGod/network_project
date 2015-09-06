/**
*@file
*@brief Http协议服务器接口的实现的头文件
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
*/
#pragma once

///<HttpServeSocket 命令目标
class HttpServeSocket : public CAsyncSocket
{
public:
	HttpServeSocket();
	virtual ~HttpServeSocket();
	virtual void OnAccept(int nErrorCode);
};


