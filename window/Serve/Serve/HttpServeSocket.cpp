// HttpServeSocket.cpp : ʵ���ļ�
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


// HttpServeSocket ��Ա����


void HttpServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	static int count = 0;
	CString Hint;
	HttpClientSocket *httpClientSocket = new HttpClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*httpClientSocket);
	Hint.Format(_T("��%d����ҳ����������"), count + 1);
	myclient[httpClientSocket] = count++;
	PrintView(Hint);
}
