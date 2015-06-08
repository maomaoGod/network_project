// HttpClientSocket.cpp : ʵ���ļ�
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


// HttpClientSocket ��Ա����

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
	Receive(Buffer, MAXLEN);//���Կͻ���%d����Ϣ: myclient[this],
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
	log.Format(_T("��������������ͻ���%d�ѶϿ�����"), myclient[this]+1);
	PrintView(log);
	CAsyncSocket::OnReceive(nErrorCode);
}


void HttpClientSocket::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString back;
	back.Format(_T("Ӧ���%d����ҳ���������ӣ���ȴ���ҳ����"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void HttpClientSocket::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString log;
	log.Format(_T("�ͻ���%d�ѶϿ�����"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
