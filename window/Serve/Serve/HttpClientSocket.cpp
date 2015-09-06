
/**
*@file
*@brief To achieve the function of server socket
*@author ACM1201
*@date 2015/04/19
*@version 1.1
*@note
*/
#pragma once

#include "stdafx.h"
#include "Serve.h"
#include "HttpClientSocket.h"
#include "Network.h"
#include "Httpworker.h"
#include <map>

using namespace std;
using namespace NetWork;
extern void PrintView(CString e);
extern map <HttpClientSocket *, int> myclient;
// HttpClientSocket



HttpClientSocket::HttpClientSocket()/**@brief HttpClientSocket构造函数 */
{
	dll = new NetDll();
	//dll->OpenDll();
	dll->LoadDll(_T("G:\\Users\\TySag\\Desktop\\tmp\\httpworker\\Debug\\httpworker.dll"));
	//httpworker = new Httpworker();
	httptr = (HttPtr)dll->GetFun("NewHttptr");
	httpmake = (HttpMake)dll->GetFun("Make");
	httpgetPort = (HttpgetPort)dll->GetFun("getPort");
	httpgethost = (Httpgethost)dll->GetFun("gethost");
	httpgetMsg = (HttpgetMsg)dll->GetFun("getMsg");
	httpsetmsg = (HttpSetMsg)dll->GetFun("setMsg");
	httpgetdata = (Httpgetdata)dll->GetFun("getdata");
	httpsetdata = (Httpsetdata)dll->GetFun("setdata");
	httpanaly = (Httpanaly)dll->GetFun("analy");
}

HttpClientSocket::~HttpClientSocket()/**@brief  HttpClientSocket析构函数 */
{
}
/**
* @brief
* @param [in] <nErrorCode> int型
* @author ACM2012
* @return 无
* @note
* HttpClientSocket 成员函数,添加专用代码和/或调用基类
* 服务器端接收客户端的数据信息，在这里我们采用固定buf来存数据。
* 先为将要存数据的数组设置一个默认值BUFSIZE，
* 如果数据长度不大于BUFSIZE，则只需要一个buf[BUFSIZE]数组即可；
* 如果数据长度大于BUF，则数据的前BUFSIZE大小的字符存入第一个buf数组中，
* 声明一个新的buf[BUFSIZE]的数组，用来存余下的数据，
* 直到所有的数据都能被存入数组中。
*/


void HttpClientSocket::OnReceive(int nErrorCode)
{
	//Httpworker *httpworker = new Httpworker();
	void* httpworker = httptr();
	if (nErrorCode){
		CAsyncSocket::OnReceive(nErrorCode);
		return;
	}
	string data;
	char buf[BUFSIZE + 1];
	while (true){
		memset(buf, 0, sizeof(buf));
		Receive(buf, BUFSIZE);
		buf[BUFSIZE] = '\0';
		data += (*new string(buf));
		if (data.find("\r\n\r\n")) break;
	}
	int xpos = data.find("\r\n\r\n");
	PrintView(STR::S2CS(data.substr(0, xpos)));
	//int len = httpworker->setMsg(data);
	int len = httpsetmsg(httpworker,data);
	if (len > 0){
		//data = httpworker->getdata();
		data = httpgetdata(httpworker);
		int count;
		while (true){
			if (len == 0) break;
			memset(buf, 0, sizeof(buf));
			count = Receive(buf, BUFSIZE);
			buf[BUFSIZE] = '\0';
			data += (*new string(buf));
			len -= count;
		}
		//httpworker->setdata(data);
		httpsetdata(httpworker,data);
	}
	//not show data
	//if (httpworker->analy() > 0){
	if (httpanaly(httpworker) > 0){
		//httpworker->Make();
		httpmake(httpworker);
	}
	//data = httpworker->getMsg();
	data = httpgetMsg(httpworker);
	PrintView(STR::S2CS(data));
	//Send(data, sizeof(TCHAR)*data.GetLength());
	Send(data.c_str(), data.length());
	//data = _T("http res end");
	//Send(data, data.GetLength()*sizeof(TCHAR));
	this->Close();
	CString log;
	log.Format(_T("服务器已主动与客户端%d已断开连接"), myclient[this]+1);
	PrintView(log);
	CAsyncSocket::OnReceive(nErrorCode);
}
/**
* @brief
* @param [in] <nErrorCode> int型
* @author ACM2012
* @return 无
* @note
* HttpClientSocket 成员函数 OnSend函数
* 发送网页数据
*/
void HttpClientSocket::OnSend(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString back;
	back.Format(_T("应答第%d次网页请求，已连接，请等待网页数据"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}
/**
* @brief
* @param [in] <nErrorCode> int型
* @author ACM2012
* @return 无
* @note
* HttpClientSocket 成员函数 OnClose函数
* 断开连接
*/
void HttpClientSocket::OnClose(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString log;
	log.Format(_T("客户端%d已断开连接"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	//delete this;
}
