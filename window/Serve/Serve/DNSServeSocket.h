#pragma once

#include "map"
#include "DNSworker.h"
using namespace std;
typedef map<string, string> dnsmap;

#define dns_port 6553
// DNSServeSocket ÃüÁîÄ¿±ê

class DNSServeSocket : public CAsyncSocket
{
public:
	DNSServeSocket();
	virtual ~DNSServeSocket();
	virtual void OnAccept(int nErrorCode); 
	void addmap(string host, string ip){
		(*dnshost)[host] = ip;
	}
	string getIP(string host){
		return (*dnshost)[host];
	}
private:
	DNSworker dnsworker;
	dnsmap *dnshost;
};


