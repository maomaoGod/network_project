// DNSServeSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DNSServeSocket.h"
#include "Tools.h"
#include "DNSClientSocket.h"
using namespace Tools;
// DNSServeSocket

using namespace std;
map <DNSClientSocket *, int> myclient;

DNSServeSocket::DNSServeSocket()
{
	dnshost = new dnsmap();
	FILE *pb;
	char temp[1024];
	string *host, *ip;
	if (!fopen_s(&pb, "dns/list.txt", "r")){
		memset(temp, 0, sizeof(temp));
		fscanf_s(pb, "%s", temp, 1024);
		host = new string(temp);
		memset(temp, 0, sizeof(temp));
		fscanf_s(pb, "%s", temp, 1024);
		ip = new string(temp);
		(*dnshost)[*host] = *ip;
		fclose(pb);
	}
}

DNSServeSocket::~DNSServeSocket()
{
	string path = "dns/list.txt";
	if (Tools::FIO::Exist(path))
		Tools::FIO::DelFile(path);
	if (dnshost->size()<1)
		return;
	FILE *fp;
	fopen_s(&fp, path.c_str(), "w");
	dnsmap::iterator it = dnshost->begin();
	for (; it != dnshost->end(); it++){
		fprintf_s(fp, "%s\n", it->first.c_str());
		fprintf_s(fp, "%s\n", it->second.c_str());
	}
	fclose(fp);
	dnshost->clear();
}

void DNSServeSocket::OnAccept(int nErrorCode)
{
	// TODO:  在此添加专用代码和/或调用基类
	static int count = 0;
	CString Hint;
	DNSClientSocket *dnsClientSocket = new DNSClientSocket();
	dnsClientSocket->dnshost = this->dnshost;
	CAsyncSocket::OnAccept(nErrorCode);
	this->Accept(*dnsClientSocket);
	myclient[dnsClientSocket] = count++;
}
// DNSServeSocket 成员函数
