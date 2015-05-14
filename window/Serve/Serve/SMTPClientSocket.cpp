// SMTPClientSocket.cpp : ʵ���ļ�
//

#pragma once
#include "stdafx.h"
#include "Serve.h"
#include "SMTPClientSocket.h"

#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <SMTPClientSocket *, int> myclient;

// SMTPClientSocket

SMTPClientSocket::SMTPClientSocket()
{
}

SMTPClientSocket::~SMTPClientSocket()
{
}


SMTP app;
// SMTPClientSocket ��Ա����
void SMTPClientSocket::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//���Կͻ���%d����Ϣ: myclient[this],
		result.Format(_T("%s"), Buffer);
		PrintView(result);
		app.CMD_Dispatch(STR::CS2S(result));
		//char *stemp;
		if (app.GetCode()==0)
		{
			Send(NULL, 0);
		}
		else
		if (app.GetCode() == OK){
			back = STR::S2CS(app.GetMsg());
			Send(back, back.GetLength()*sizeof(TCHAR));
			Send(NULL, 0);
		}
		else{
			back.Format(_T("%d"), app.GetCode());
			back = back + _T("\r\n") + STR::S2CS(app.GetMsg());
			Send(back, back.GetLength()*sizeof(TCHAR));
			Send(NULL, 0);
		}
		PrintView(back);
		//back.Format(_T("�ɹ�������Ϣ:%s"), Buffer);
		//Send(back, back.GetLength()*sizeof(TCHAR));
		//Send(NULL, 0);
		//Exit the Socket
		if (app.GetCode() == SMTP_QUIT)  this->Close();
	}
	CAsyncSocket::OnReceive(nErrorCode);
}
void SMTPClientSocket::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString back;
	back.Format(_T("�����ӷ�����"));
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void SMTPClientSocket::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString log;
	log.Format(_T("�ͻ���%d�ѶϿ�����"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
