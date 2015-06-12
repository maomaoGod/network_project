#pragma once
#include <string>
#include <vector>
#include <map>
#include "Tools.h"

using namespace Tools;
using namespace std;

/**/
typedef string version;
typedef map<string, int> checkarg;
typedef map<string, string> resword;

#define MarkHttp "\r\n"

/* Send Msg and RMsg
Msg:

RMsg:

cookie:

hit: web cache use if-moditied-since
*/

struct Httpcookie{
	int num;
	string history;
	int RandCook(){
		num = 1000 + rand() % 9000;
		return num;
	}
	bool findcookie(){
		string path = "cache/cookie/" + STR::int2string(num) + ".cookie";
		return FIO::Exist(path);
	}
	
	bool create(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";
		if (FIO::Exist(path)){
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;
		FIO::SaveFile(path, &w);
		return true;
	}
	bool create_host(string host){
		string path = "cache/cookie/" + host + ".cookie";
		if (FIO::Exist(path)){
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;
		FIO::SaveFile(path, &w);
		return true;
	}
	
	bool readcook(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";
		if (!FIO::Exist(path)){
			return false;
		}
		string r = FIO::ReadFile(path, 0);
		vector<string> d;
		STR::Split(r, &d, '|');
		num = STR::string2int(d[0]);
		history = d[1];
		return true;
	}
	bool readcook_host(string host = "", int cookie = 0){
		string path = "cache/cookie/" + host + ".cookie";
		if (!FIO::Exist(path)){
			return false;
		}
		string r = FIO::ReadFile(path, 0);
		vector<string> d;
		STR::Split(r, &d, '|');
		num = STR::string2int(d[0]);
		history = d[1];
		return true;
	}
};

struct HttpRMsg{
	//request
	struct{
		string method;
		string path;
		version no;
	};
	//host
	struct{
		string host;
		string user;
		int length;
		string language;
		string connect;
		string if_modified_since;
		string accept;
		string accept_enconding;
	};
	Httpcookie cookie;
	string data;
	void setCode(string msg){
		vector<string> d;
		STR::Split(msg, &d, ' ');
		method = d[0];
		int len = d[1].length();
		if (d[1][len - 1] == '/') {
			path = d[1] + "index.html";
		}
		else path = d[1];
		no = d[2];
	}
	void findHead(string msg){
		vector<string> d;
		STR::Split(msg, &d, ": ");
		if (d.size() < 2) return;
		if (d[0] == "Accept")
			accept = d[1];
		else if (d[0] == "Accept-Encoding"){
			accept_enconding = d[1];
		}
		else if (d[0] == "Accept-Language")
			language = STR::string2int(d[1]);
		else if (d[0] == "Connection")
			connect = d[1];
		/*
		else if (d[0] == "Cookie")
			connect = d[1];*/
		else if (d[0] == "Host")
			host = d[1];
		else if (d[0] == "User-Agent")
			user = d[1];
		else if (d[0] == "If_modified_since")
			if_modified_since = d[1];
		else if (d[0] == "Content-Length")
			length = STR::string2int(d[1]);
	
	}
};

struct HttpMsg{
	struct{
		version no;
		string code;
		string word;
	};
	struct{
		string connect;
		string date;
		string server;
		string last_modified;
		int length;
		vector<string> type;
		string vary;
		string accapt_ranges;
	};
	Httpcookie cookie;
	string data;
	void setCode(string msg){
		vector<string> d;
		STR::Split(msg, &d, ' ');
		no = d[0];
		code = d[1];
		word = d[2];
	}
	void findHead(string msg){
		vector<string> d;
		STR::Split(msg, &d, ": ");
		if (d.size() < 2) return;
		if (d[0] == "Date")
			date = d[1];
		else if (d[0] == "Content-Type"){
			type.push_back(d[1]);
		}
		else if (d[0] == "Content-Length")
			length = STR::string2int(d[1]);
		else if (d[0] == "Last-Modified")
			last_modified = d[1];
		else if (d[0] == "Connection")
			connect = d[1];
		else if (d[0] == "Vary")
			vary = d[1];
		else if (d[0] == "Set-Cookie"){
			cookie.history = d[1];//set cookie
		}
		else if (d[0] == "Server")
			server = d[1];
		else if (d[0] == "Accept-Ranges")
			accapt_ranges = d[1];
	}
};
/*
	version no;
	string code;
	string word;
	string connect;
	string date;
	string server;
	string last_modified;
	int length;
	string type;
	Httpcookie cookie;
	string data;

*/

class Httpworker
{
public:
	Httpworker();
	~Httpworker();
	void Make();
	int analy();
	void check();
	//rmsg = (HttpRMsg *)rec;
	/*
	//request
	string method;
	string path;
	version no;
	//host
	string host;
	string user;
	string language;
	string connect;
	string if_modified_since;
	Httpcookie *cookie;
	string options;
	string data;*/
	int setMsg(string rec);

	string getMsg();

	string gethost(){
		return host;
	}
	//return all string in msg
	string look_msg();
	//return all string in rmsg
	string look_rmsg();

	CString show_rmsg();


	void setdata(string msg){
		rmsg->data = msg;
	}
	string getdata(){
		return rmsg->data;
	}

private:
	void GET();
	/**
	*@brief the function we choose to deal with the data request like get
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*与get方法相同，服务器只返回状态行和头标，
	*并不返回请求文档, Map it to 2
	*path >> data[1]
	*host >> data[2]
	*1 find the file according to the path
	*2 if not exist return (NO_FOUND,"NOT FOUND")
	given the Msg like (no of state, and text Msg)
	use sender to send it out
	*/
	void HEAD();
	/**
	*@brief POST methods
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*服务器接受被写入客户端输出流中的数据的请求,Map it to 3
	*path >> data[1]
	*host >> data[2]
	*1 analysis the data from client
	*2 save it the Socket conn to it
	*return OK
	*/
	void POST();
	/**
	*@brief PUT method
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*服务器保存请求数据作为指定URI新内容的请求,Map it to 4
	*path >> data[1]
	*host >> data[2]
	*1 find the file in path?
	*2 no -> save it in this path ,return OK
	*3 yes ->
	* 3.1 rewrite it in this path , return OK
	* 3.2 return HAVE_EXISTED
	*/
	void PUT();
	/**
	*@brief DELETE some options of request methods
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*服务器删除URI中命名的资源的请求, Map it to 5
	*path >> data[1]
	*host >> data[2]
	*1 find the file in path?
	*2 yes -> delete it in this path ,return OK
	*3 no ->
	* 3.1 return OK anyway
	* 3.2 return warnning "NOT_EXIST"
	*/
	void DELETEFILE();
	/**
	*@brief some options of request methods
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*关于服务器支持的请求方法信息的请求,Map it to 6
	*just tell the people things this serve support
	* 1 return OK
	* 2 return "get HEAD POST PUT DELETE OPTIONS TRACE CONNECT"as Msg
	*/
	void OPTIONS();
	/**
	*@brief don't Know what's it meaning
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*Web服务器反馈Http请求和其头标的请求,Map it to 7
	*/
	void TRACE_HTTP();
	/**
	*@brief NOT USE FUNCTION
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*已文档化但当前未实现的一个方法,预留做隧道处理, Map it to 8
	*this function leaved to satifiy other function
	*and it doesn't has a function now
	*/
	void CONNECT();
	
	void BAD_RES();

	void ERROR_HTTP();

	typedef void(Httpworker::*DealWithFunciton)();
	map<string, DealWithFunciton> *fun;
	HttpMsg *msg;
	HttpRMsg *rmsg;
	checkarg *alg;
	resword *word;
	string host;
};

