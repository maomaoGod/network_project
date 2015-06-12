// FtpTranSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Serve.h"
#include "FtpTranSocket.h"
#include "map"
using namespace std;

extern void PrintView(CString e);
extern map <FtpTranSocket *, int> myclient;
// FtpTranSocket

FtpTranSocket::FtpTranSocket()
{
}

FtpTranSocket::~FtpTranSocket()
{
}


// FtpTranSocket ��Ա����

void FtpTranSocket::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString rev, temp;
	TCHAR buf[1024];
	if (nErrorCode == 0){
		//re
		//send
		if (!style){//download
			memset(buf, 0, sizeof(buf));
			while (Receive((void *)buf, 1024)){
				temp.Format(_T("%s"), buf);
				rev += temp;
				memset(buf, 0, 1024 * sizeof(TCHAR));
			}
			FIO::SaveFile(des, &STR::CS2S(rev));
			CString sendmsg = _T("null");
			Send(sendmsg, sizeof(TCHAR)*sendmsg.GetLength());
		}
		else{
			memset(buf, 0, sizeof(buf));
			Receive((void *)buf, 1024);
			rev = STR::S2CS(FIO::ReadFile(src, 1));
			Send(rev, sizeof(TCHAR)*rev.GetLength());
		}
	}
	this->Close();
	CAsyncSocket::OnReceive(nErrorCode);
}


void FtpTranSocket::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString back;
	back.Format(_T("�����ӵ�������"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void FtpTranSocket::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString log;
	log.Format(_T("�ͻ���%d�ѶϿ�����"), myclient[this] + 1);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
