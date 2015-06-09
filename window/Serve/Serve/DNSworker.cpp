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
QR(1����)����ѯ/��Ӧ�ı�־λ��1Ϊ��Ӧ��0Ϊ��ѯ��
opcode(4����)�������ѯ����Ӧ������(��Ϊ0���ʾ�Ǳ�׼�ģ���Ϊ1���Ƿ���ģ���Ϊ2���Ƿ�����״̬����)��
AA(1����)����Ȩ�ش�ı�־λ����λ����Ӧ��������Ч��1��ʾ���ַ�������Ȩ�޷�����(����Ȩ�޷������Ժ�������)
TC(1����)���ضϱ�־λ��1��ʾ��Ӧ�ѳ���512�ֽڲ��ѱ��ض�(��ϡ����ǵ������������ضϺ�UDP�йأ��ȼ���)
RD(1����)����λΪ1��ʾ�ͻ���ϣ���õ��ݹ�ش�(�ݹ��Ժ�������)
RA(1����)��ֻ������Ӧ��������Ϊ1����ʾ���Եõ��ݹ���Ӧ��
zero(3����)����˵Ҳ֪������0�ˣ������ֶΡ�
rcode(4����) :�����룬��ʾ��Ӧ�Ĳ��״̬��ͨ��Ϊ0��3
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


