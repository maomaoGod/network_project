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
	// TODO:  �ڴ����ר�ô����/����û���
	CAsyncSocket::OnConnect(nErrorCode);
}


void SocketListen::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	SocketManager *pclientmanager = new SocketManager();
	Accept(*pclientmanager);
	CAsyncSocket::OnAccept(nErrorCode);
}
