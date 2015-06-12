// FtpTranSocket.cpp : 实现文件
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


// FtpTranSocket 成员函数

void FtpTranSocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
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
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("已连接到服务器"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void FtpTranSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this] + 1);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
