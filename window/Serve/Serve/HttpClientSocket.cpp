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
	CString data;
	//Receive(data, sizeof(HttpRMsg));
	memset(Buffer, 0, sizeof(Buffer));
	Receive(Buffer, MAXLEN);//来自客户端%d的消息: myclient[this],
	data.Format(_T("%s"), Buffer);
	string msg = STR::CS2S(data);
	if (!httpworker->setMsg(msg)) {
		PrintView(data);
		PrintView(_T("Wrong Msg"));
	}
	PrintView(httpworker->show_rmsg());
	if (httpworker->analy() > 0){
		httpworker->Make();
	}
	data = STR::S2CS(httpworker->getMsg());
	//PrintView(data);
	Send(data, sizeof(TCHAR)*data.GetLength());
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
	Send(back, back.GetLength()*sizeof(TCHAR));
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
