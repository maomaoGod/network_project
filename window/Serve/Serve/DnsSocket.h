#pragma once

#include "map"
#include "DNSworker.h"

using namespace std;
typedef map<string, string> dnsmap;
// DnsSocket ÃüÁîÄ¿±ê
#define dns_port 6553
class DnsSocket : public CAsyncSocket
{
public:
	DnsSocket();
	virtual ~DnsSocket();
	virtual void OnReceive(int nErrorCode);
	void addmap(string host, string ip){
		(*dnshost)[host] = ip;
	}
	string getIP(string host){
		return (*dnshost)[host];
	}
	static DWORD WINAPI  ReadThread(LPARAM lParam);
private:
	dnsmap *dnshost;
	void ReceiveData();
	bool finish;
	DNSworker dnsworker;
};


