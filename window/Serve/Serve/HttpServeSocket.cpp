// HttpServeSocket.cpp : 实现文件
//
/**@file
 *@brief To achieve the function of server socket
 *@author xinYao,jinxiaFang
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *XXXXXXX
 */
#pragma once

#include "stdafx.h"
#include "Serve.h"
#include "HttpServeSocket.h"
#include "HttpClientSocket.h"
#include <map>

extern void PrintView(CString);

using namespace std;
map <HttpClientSocket *,int> myclient;

// HttpServeSocket

HttpServeSocket::HttpServeSocket()
{
}

HttpServeSocket::~HttpServeSocket()
{
}


// HttpServeSocket 成员函数


void HttpServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	HttpClientSocket *httpClientSocket = new HttpClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*httpClientSocket);
	Hint.Format(_T("第%d次网页请求已连接"), count + 1);
	myclient[httpClientSocket] = count++;
	PrintView(Hint);
}
