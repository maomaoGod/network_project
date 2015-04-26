// mycsocket.cpp : 实现文件
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


// mycsocket 成员函数


void mycsocket::OnReceive(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类

	CSocket::OnReceive(nErrorCode);
}
