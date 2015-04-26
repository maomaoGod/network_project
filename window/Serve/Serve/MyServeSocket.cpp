// MyServeSocket.cpp : ʵ���ļ�
//
/**@file
 *@brief To achieve the function of server socket
 *@author xinYao,jinxiaFang
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *XXXXXXX
 */

#include "stdafx.h"
#include "Serve.h"
#include "MyServeSocket.h"
#include  "ClientSocket.h"
#include <map>

extern void PrintView(CString);

using namespace std;
map <ClientSocket *,int> myclient;



// MyServeSocket

MyServeSocket::MyServeSocket()
{
}

MyServeSocket::~MyServeSocket()
{
}


// MyServeSocket ��Ա����


void MyServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	static int count = 0;
	CString Hint;
	ClientSocket  *clientsocket = new ClientSocket();
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*clientsocket);
	Hint.Format(_T("��%d����ҳ����������"), count + 1);
	myclient[clientsocket] = count++;
	PrintView(Hint);
}
