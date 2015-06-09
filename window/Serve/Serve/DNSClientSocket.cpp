// DNSClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DNSClientSocket.h"
#include "DNSworker.h"
#include "DNSServeSocket.h"
#include "Tools.h"
// DNSClientSocket

using namespace std;

DNSClientSocket::DNSClientSocket()
{
}

DNSClientSocket::~DNSClientSocket()
{
}


// DNSClientSocket 成员函数
void DNSClientSocket::OnReceive(int nErrorCode)
{
	DNSworker dnsworker;
	if (nErrorCode){
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	TCHAR szRecValue[1024];
	memset(szRecValue, 0, sizeof(szRecValue));
	CString temp;
	Receive(szRecValue, 1024);
	temp.Format(_T("%s"), szRecValue);
	dnsworker.setMsg(Tools::STR::CS2S(temp));
	if (dnsworker.Analy() < 0){
		Send(temp, sizeof(TCHAR)*temp.GetLength());
	}
	else if (dnshost->find(dnsworker.gethost()) == dnshost->end()){
		Send(temp, sizeof(TCHAR)*temp.GetLength());
	}
	else{
		dnsworker.Response((*dnshost)[dnsworker.gethost()]);
		temp = Tools::STR::S2CS(dnsworker.getMsg());
		Send(temp, sizeof(TCHAR)*temp.GetLength());
	}
	this->Close();
	CAsyncSocket::OnReceive(nErrorCode);
}

void DNSClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back = _T("NULL");
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void DNSClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
