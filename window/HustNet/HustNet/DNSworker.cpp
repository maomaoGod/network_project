#include "stdafx.h"
#include "DNSworker.h"
#include "Tools.h"

using namespace Tools;

DNSworker::DNSworker()
{
	//do nothing
}

DNSworker::~DNSworker()
{
	if (msg != NULL) delete msg;
}

void DNSworker::Make(string host, string dns_ip, string options){
	if (msg != NULL) delete msg;
	if (dns_ip == "") dns_ip = DNSworker::getdefault_dfs();
	msg = new DNSmsg();
	msg->ask = host; // set host
	msg->ip = ""; // get ip from host
	msg->dns_auth = dns_ip; // set dns_ip
	msg->others = options; // set options
	msg->problem_num = 1;
	msg->auth_num = 1;
	msg->ans_num = 0;
	msg->other_num = options.size();
	msg->mark_num = 0;
	msg->mark <<= 1;// QR = 0;
	msg->mark <<= 4;// opcode = 0;
	msg->mark <<= 1;// AA = 0
	msg->mark <<= 1;// TC = 0
	msg->mark <<= 1;// RA = 0
	msg->mark <<= 3;//zero = 0
	msg->mark <<= 4;//rcode = 0
}

dns_error_code DNSworker::Analy(){
	if (! (msg->mark >> 15 & 1)){// QR = 0
		return -1;// error msg
	}
	if (!(msg->mark >> 11) & 1){// AA
		return -1;// error serves
	}
	if (!(msg->mark >> 10) & 1){
		return -1;// msg being cut
	}
	if (msg->mark & 0x000f){
		return -1;// true error
	}
	this->ip_ans = msg->ip;
}

string DNSworker::getMsg(){
	string look;
	string a, b, c, d, e;
	a = STR::int2string(msg->mark);
	b = STR::int2string(msg->problem_num);
	c = STR::int2string(msg->ans_num);
	d = STR::int2string(msg->auth_num);
	e = STR::int2string(msg->other_num); 
	if (msg->ask == "") msg->ask = "null";
	if (msg->ip == "") msg->ip = "null";
	if (msg->dns_auth == "") msg->dns_auth = "null";
	if (msg->others == "") msg->others = "null";
	look = a + "\n" + b + "\n" + c + "\n" + d + "\n" + e + "\n";
	look += msg->ask + "\n" + msg->ip + "\n" + msg->dns_auth + "\n" + msg->others + "\n";
	return look;
}

void DNSworker::setMsg(string rec){
	vector<string> d;
	msg = new DNSmsg();
	STR::Split(rec, &d, '\n');
	msg->mark = STR::string2int(d[0]);
	msg->problem_num = STR::string2int(d[1]);
	msg->ans_num = STR::string2int(d[2]);
	msg->auth_num = STR::string2int(d[3]);
	msg->other_num = STR::string2int(d[4]); 
	if (d[5] == "null") msg->ask = "";
	else msg->ask = d[5];
	if (d[6] == "null") msg->ip = "";
	else msg->ip = d[6];
	if (d[7] == "null") msg->dns_auth = "";
	else msg->dns_auth = d[7];
	if (d[8] == "null") msg->others = "";
	else msg->others = d[8];
	return;
}
