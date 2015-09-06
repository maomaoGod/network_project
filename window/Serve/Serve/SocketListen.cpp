#include "stdafx.h"
#include "SocketListen.h"


SocketListen::SocketListen()
{
	AfxSocketInit();
}


SocketListen::~SocketListen()
{

}


void SocketListen::OnConnect(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CAsyncSocket::OnConnect(nErrorCode);
}


void SocketListen::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	SocketManager *pclientmanager = new SocketManager();
	Accept(*pclientmanager);
	CAsyncSocket::OnAccept(nErrorCode);
}
