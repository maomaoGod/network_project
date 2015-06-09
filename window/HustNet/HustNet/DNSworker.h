#pragma once
#include <string>
using namespace std;
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
|-----------------------------|
|    ��ʶ��      |     ��־     |
|    ������      |   �ش�RR��   |
|   Ȩ��RR��     |   ����RR��   |
|-----------------------------|
|            ��������          |
|            �ش�����          |
|            Ȩ������          |
|            ��������          |
|-----------------------------|
*/
typedef unsigned char byte;
typedef unsigned short head_16b;
typedef unsigned int head_32b;
typedef unsigned int dns_error_code;
typedef string IP;

#define dns_port 6553

struct DNSmsg{
	head_16b mark_num;
	head_16b mark;
	head_16b problem_num;
	head_16b ans_num;
	head_16b auth_num;
	head_16b other_num;
	string ask;
	IP ip;
	string dns_auth;
	string others;
};

class DNSworker
{
private:
public:
	DNSworker();
	~DNSworker();
	void Make(string host, string dns_ip = "", string options = "");
	dns_error_code Analy();
	IP Getip(){
		return ip_ans;
	}

	string getMsg();
	void setMsg(string msg);//msg = new DNSmsg();

	static string getdefault_dfs(){
		return "127.0.0.1";
	}
private:
	DNSmsg *msg;
	IP ip_ans;
};

