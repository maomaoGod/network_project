// DnsSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Serve.h"
#include "DnsSocket.h"
#include "Tools.h"

using namespace Tools;

// DnsSocket

DnsSocket::DnsSocket()
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
	}
	finish = true;
	CreateThread(NULL, NULL,(LPTHREAD_START_ROUTINE)ReadThread, (LPVOID)this, NULL, NULL);
	fclose(pb);
}

DWORD WINAPI  DnsSocket::ReadThread(LPARAM lParam)
{
	DnsSocket *pthis = (DnsSocket *)lParam;
	pthis->ReceiveData();
	return 0;
}

void DnsSocket::ReceiveData()
{
	TCHAR szRecValue[1024];
	UINT c_port;
	CString c_ip;
	CString temp;
	while (finish){
		memset(szRecValue, 0, sizeof(szRecValue));
		while (ReceiveFromEx(szRecValue, 1024, c_ip, c_port)==-1){
			;
		}
		temp.Format(_T("%s"), szRecValue);
		dnsworker.setMsg(Tools::STR::CS2S(temp));
		if (dnsworker.Analy() < 0){
			SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
			return;
		}
		if (dnshost->find(dnsworker.gethost()) == dnshost->end()){
			SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
			return;
		}
		dnsworker.Response((*dnshost)[dnsworker.gethost()]);
		temp = Tools::STR::S2CS(dnsworker.getMsg());
		SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
	}
}

DnsSocket::~DnsSocket()
{
	finish = false;
	string path = "dns/list.txt";
	if (Tools::FIO::Exist(path)) 
		Tools::FIO::DelFile(path);
	if (dnshost->size()<1) 
		return;
	FILE *fp;
	fopen_s(&fp, path.c_str(), "w");
	dnsmap::iterator it = dnshost->begin();
	for (; it != dnshost->end(); it++){
		fprintf_s(fp, "%s\n",it->first.c_str());
		fprintf_s(fp, "%s\n", it->second.c_str());
	}
	fclose(fp);
	dnshost->clear();
}

void DnsSocket::OnReceive(int nErrorCode){
	//
	TCHAR szRecValue[1024];
	memset(szRecValue, 0, sizeof(szRecValue));
	UINT c_port;
	CString c_ip;
	CString temp;
	if (!nErrorCode){
		ReceiveFromEx(szRecValue, 1024, c_ip, c_port);
	    temp.Format(_T("%s"), szRecValue);
		dnsworker.setMsg(Tools::STR::CS2S(temp));
	    if (dnsworker.Analy() < 0){
			SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
		    return;
	    }
		if (dnshost->find(dnsworker.gethost())==dnshost->end()){
			SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
		    return;
	    }
		dnsworker.Response((*dnshost)[dnsworker.gethost()]);
		temp = Tools::STR::S2CS(dnsworker.getMsg());
		SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), c_port, c_ip);
	}
	CAsyncSocket::OnReceive(nErrorCode);
}

// DnsSocket 成员函数
