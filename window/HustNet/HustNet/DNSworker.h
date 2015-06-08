#pragma once
#include <string>
using namespace std;
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
|-----------------------------|
|    标识数      |     标志     |
|    问题数      |   回答RR数   |
|   权威RR数     |   附加RR数   |
|-----------------------------|
|            问题区域          |
|            回答区域          |
|            权威区域          |
|            附加区域          |
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

