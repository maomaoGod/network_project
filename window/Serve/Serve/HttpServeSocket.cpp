/**@file
 *@brief To achieve the function of server socket
 *@author ACM1201
 *@date 2015/04/19
 *@version 1.1
 *@note
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

HttpServeSocket::HttpServeSocket()/**@brief HttpServeSocket构造函数 */
{
}

HttpServeSocket::~HttpServeSocket()/**@brief  HttpServeSocket析构函数 */
{
}


// HttpServeSocket 成员函数


/**
* @brief
* @param [in] <nErrorCode> int型
* @author ACM2012
* @return 无
* @note
* HttpServeSocket 成员函数,添加专用代码和/或调用基类
*/
void HttpServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	HttpClientSocket *httpClientSocket = new HttpClientSocket();
	CmyAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*httpClientSocket);
	Hint.Format(_T("第%d次网页请求已连接"), count + 1);
	myclient[httpClientSocket] = count++;
	PrintView(Hint);
}
