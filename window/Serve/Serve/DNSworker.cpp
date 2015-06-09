#include "stdafx.h"
#include "DNSworker.h"
#include "Tools.h"

using namespace Tools;


DNSworker::DNSworker()
{
	//do nothing
	msg = new DNSmsg();
}

DNSworker::~DNSworker()
{
	if (msg != NULL) delete msg;
}

void DNSworker::Response(IP ip, string options){
	msg->mark_num = 1;
	msg->problem_num = 0;
	msg->ans_num = 1;
	msg->auth_num = 1;
	msg->other_num = options.size();
	// ask = host , not change
	msg->ip = ip;//return ip
	string dns_auth;
	msg->others = options;
	// AA = 1
	msg->mark = msg->mark | 0x8000;
	//rcode = 0 3
	msg->mark = msg->mark | 0x0000;
}

dns_error_code DNSworker::Analy(){
	if ((msg->mark >> 15 & 1)){// QR = 1
		return -1;// error msg
	}
	if (!(msg->mark >> 10) & 1){
		return -1;// msg being cut
	}
	if ((msg->mark >> 9) & 1){//16 4444 -> 0x0080
		msg->mark = msg->mark | 0x0080;// RA = 1
	}
	return 1;// right message to do something
}

/*
[mark]:
QR(1比特)：查询/响应的标志位，1为响应，0为查询。
opcode(4比特)：定义查询或响应的类型(若为0则表示是标准的，若为1则是反向的，若为2则是服务器状态请求)。
AA(1比特)：授权回答的标志位。该位在响应报文中有效，1表示名字服务器是权限服务器(关于权限服务器以后再讨论)
TC(1比特)：截断标志位。1表示响应已超过512字节并已被截断(依稀好像记得哪里提过这个截断和UDP有关，先记着)
RD(1比特)：该位为1表示客户端希望得到递归回答(递归以后再讨论)
RA(1比特)：只能在响应报文中置为1，表示可以得到递归响应。
zero(3比特)：不说也知道都是0了，保留字段。
rcode(4比特) :返回码，表示响应的差错状态，通常为0和3
[end mark]
*/

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


