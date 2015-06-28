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

HttpServeSocket::HttpServeSocket()/**@brief HttpServeSocket���캯�� */
{
}

HttpServeSocket::~HttpServeSocket()/**@brief  HttpServeSocket�������� */
{
}


// HttpServeSocket ��Ա����


/**
* @brief
* @param [in] <nErrorCode> int��
* @author ACM2012
* @return ��
* @note
* HttpServeSocket ��Ա����,���ר�ô����/����û���
*/
void HttpServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	static int count = 0;
	CString Hint;
	HttpClientSocket *httpClientSocket = new HttpClientSocket();
	CmyAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*httpClientSocket);
	Hint.Format(_T("��%d����ҳ����������"), count + 1);
	myclient[httpClientSocket] = count++;
	PrintView(Hint);
}
