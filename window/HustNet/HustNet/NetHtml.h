/**@file
*@brief  网页获取类
*@date 2015.4.20
*@version <0.1>
*@note
*界面控制的实现
*/
#pragma once
#include "afxinet.h"
/**
*@class <NetHtml>
*@brief  测试获取网页类，该类实现的功能是通过套接字获取网络上的url资源
*@author ACM2012
*@note  NetHtml为测试获取网页类，该类实现的功能是通过套接字获取网络上的url资源。类中仅有一个成员函数
*getURLContext,该函数的功能是实现通过url获取对应的网页文本，并将文本转码为Unicode，返回给调用函数。
*/
class NetHtml
{
public:
	NetHtml();
	~NetHtml();
	/**@brief   通过url获取html字符串数据流*/
	CString  NetHtml::getURLContext(LPCTSTR pstrURL);
};

