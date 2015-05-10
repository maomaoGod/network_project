// FTPClientSocket.cpp : ʵ���ļ�
//

#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "FTPClientSocket.h"

#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <FTPClientSocket *, int> myclient;

// FTPClientSocket

FTPClientSocket::FTPClientSocket()
{
}

FTPClientSocket::~FTPClientSocket()
{
}


AppLayerFtp app;
// FTPClientSocket ��Ա����
void FTPClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	vector<string> path;
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//���Կͻ���%d����Ϣ: myclient[this],
		result.Format(_T("%s"), Buffer);
		PrintView(result);
		app.GetData(STR::CS2S(result));
		app.DealWith(app.GetCMD());
		//char *stemp;
		back.Format(_T("%d"), app.GetCode());
		back = back + _T("\r\n") + STR::S2CS(app.GetResMsg());
		PrintView(back);
		//back.Format(_T("�ɹ�������Ϣ:%s"), Buffer);
		Send(back, back.GetLength()*sizeof(TCHAR));
		Send(NULL, 0);
		//Exit the Socket
		if (app.GetCode() == QUIT_EXIT)  this->Close();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

void FTPClientSocket::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString back;
	back.Format(_T("��ͨ���������ӷ�����"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void FTPClientSocket::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString log;
	log.Format(_T("�ͻ���%d�ѶϿ�����"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
