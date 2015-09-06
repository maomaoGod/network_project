
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



HttpClientSocket::HttpClientSocket()/**@brief HttpClientSocket���캯�� */
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

HttpClientSocket::~HttpClientSocket()/**@brief  HttpClientSocket�������� */
{
}
/**
* @brief
* @param [in] <nErrorCode> int��
* @author ACM2012
* @return ��
* @note
* HttpClientSocket ��Ա����,���ר�ô����/����û���
* �������˽��տͻ��˵�������Ϣ�����������ǲ��ù̶�buf�������ݡ�
* ��Ϊ��Ҫ�����ݵ���������һ��Ĭ��ֵBUFSIZE��
* ������ݳ��Ȳ�����BUFSIZE����ֻ��Ҫһ��buf[BUFSIZE]���鼴�ɣ�
* ������ݳ��ȴ���BUF�������ݵ�ǰBUFSIZE��С���ַ������һ��buf�����У�
* ����һ���µ�buf[BUFSIZE]�����飬���������µ����ݣ�
* ֱ�����е����ݶ��ܱ����������С�
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
	log.Format(_T("��������������ͻ���%d�ѶϿ�����"), myclient[this]+1);
	PrintView(log);
	CAsyncSocket::OnReceive(nErrorCode);
}
/**
* @brief
* @param [in] <nErrorCode> int��
* @author ACM2012
* @return ��
* @note
* HttpClientSocket ��Ա���� OnSend����
* ������ҳ����
*/
void HttpClientSocket::OnSend(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString back;
	back.Format(_T("Ӧ���%d����ҳ���������ӣ���ȴ���ҳ����"), myclient[this] + 1);
	Send(back, back.GetLength()*sizeof(TCHAR));
	CAsyncSocket::OnSend(nErrorCode);
}
/**
* @brief
* @param [in] <nErrorCode> int��
* @author ACM2012
* @return ��
* @note
* HttpClientSocket ��Ա���� OnClose����
* �Ͽ�����
*/
void HttpClientSocket::OnClose(int nErrorCode)
{
	// TODO:  �ڴ����ר�ô����/����û���
	CString log;
	log.Format(_T("�ͻ���%d�ѶϿ�����"), myclient[this]);
	PrintView(log);
	CAsyncSocket::OnClose(nErrorCode);
	//delete this;
}
