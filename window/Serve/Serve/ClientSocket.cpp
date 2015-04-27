// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "ClientSocket.h"
#include "Network.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <ClientSocket *, int> myclient;
// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 成员函数

void ClientSocket::OnReceive(int nErrorCode)
{
// TODO:  在此添加专用代码和/或调用基类
	AppLayerHttp app;
	vector<string> path;
	if (nErrorCode == 0){
		CString result, back;
		memset(Buffer, 0, sizeof(Buffer));
		Receive(Buffer, MAXLEN);//来自客户端%d的消息: myclient[this],
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
		//back.Format(_T("成功接收消息:%s"), Buffer);
		Send(back, back.GetLength()*sizeof(TCHAR));
		this->Close();
	}
CAsyncSocket::OnReceive(nErrorCode);
}


void ClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("应答第%d次网页请求，已连接，请等待网页数据"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}

void ClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	delete this;
}
