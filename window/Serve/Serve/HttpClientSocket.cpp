// HttpClientSocket.cpp : 实现文件
//
#pragma once

#include "stdafx.h"
#include "Serve.h"
#include "HttpClientSocket.h"
#include "Network.h"
#include "Httpworker.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <HttpClientSocket *, int> myclient;
// HttpClientSocket

HttpClientSocket::HttpClientSocket()
{
}

HttpClientSocket::~HttpClientSocket()
{
}


// HttpClientSocket 成员函数

void HttpClientSocket::OnReceive(int nErrorCode)
{
	Httpworker *httpworker = new Httpworker();
	if (nErrorCode){
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	string data;
	char buf[BUFSIZE + 1];
	while (true){
		memset(buf, 0, sizeof(buf));
		Receive(buf, BUFSIZE);
		buf[BUFSIZE] = '\0';
		data += (*new string(buf));
		if (data.find("\r\n\r\n")) break;
	}
	int xpos = data.find("\r\n\r\n");
	PrintView(STR::S2CS(data.substr(0, xpos - 1)));
	int len = httpworker->setMsg(data);
	if (len > 0){
		data = httpworker->getdata();
		int count;
		while (true){
			if (len == 0) break;
			memset(buf, 0, sizeof(buf));
			count = Receive(buf, BUFSIZE);
			buf[BUFSIZE] = '\0';
			data += (*new string(buf));
			len -= count;
		}
		httpworker->setdata(data);
	}
	//not show data
	if (httpworker->analy() > 0){
		httpworker->Make();
	}
	data = httpworker->getMsg();
	//PrintView(data);
	//Send(data, sizeof(TCHAR)*data.GetLength());
	Send(data.c_str(), data.length());
	//data = _T("http res end");
	//Send(data, data.GetLength()*sizeof(TCHAR));
	this->Close();
	CString log;
	log.Format(_T("服务器已主动与客户端%d已断开连接"), myclient[this]+1);
	PrintView(log);
	CAsyncSocket::OnReceive(nErrorCode);
}


void HttpClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("应答第%d次网页请求，已连接，请等待网页数据"), myclient[this] + 1);
	//Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void HttpClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
