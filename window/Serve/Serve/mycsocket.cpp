// mycsocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Serve.h"
#include "mycsocket.h"


// mycsocket

mycsocket::mycsocket()
{
}

mycsocket::~mycsocket()
{
}


// mycsocket ��Ա����


void mycsocket::OnReceive(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���

	CSocket::OnReceive(nErrorCode);
}
