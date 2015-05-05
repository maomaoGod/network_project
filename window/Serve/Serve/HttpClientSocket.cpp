// HttpClientSocket.cpp : ʵ���ļ�
//
#pragma once

#include "stdafx.h"
#include "Serve.h"
#include "HttpClientSocket.h"
#include "Network.h"
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
// TODO:  �ڴ����ר�ô����/����û���
	AppLayerHttp app;
	vector<string> path;
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//���Կͻ���%d����Ϣ: myclient[this],
		result.Format(_T("%s"), Buffer);
		PrintView(result);
		app.GetData(STR::CS2S(result));
		app.DealWith(app.GetCMD());
		//back = STR::ANSIToUnicode(app.GetResMsg());
		char *stemp;
		if (app.GetCode() != MSG_OK){
			back.Format(_T("%d"),app.GetCode());
		}
		else {
			back = STR::S2CS(app.GetResMsg());
		}
		//PrintView(back);
		//back.Format(_T("�ɹ�������Ϣ:%s"), Buffer);
		Send(back, back.GetLength()*sizeof(TCHAR));
		this->Close();
	}
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
