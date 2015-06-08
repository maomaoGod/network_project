#pragma once
#include <string>
#include <vector>
#include <map>
#include "Tools.h"

using namespace Tools;
using namespace std;

/**/
typedef unsigned short version;
typedef unsigned int http_error_code;
typedef unsigned int http_code;
typedef map<string, int> checkarg;
typedef map<int, string> resword;

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
	string method;
	string path;
	version no;
	//host
	string host;
	string user;
	string language;
	string connect;
	string if_modified_since;
	Httpcookie cookie;
	string options;
	string data;
};

struct HttpMsg{
	version no;
	http_code code;
	string word;
	string connect;
	string date;
	string server;
	string last_modified;
	int length;
	string type;
	Httpcookie cookie;
	string data;
};


class Httpworker
{
public:
	Httpworker();
	~Httpworker();
	void Make();
	http_error_code analy();
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
	bool setMsg(string rec);

	string getMsg(){
		return look_msg();
	}
	string gethost(){
		return host;
	}
	//return all string in msg
	string look_msg();
	//return all string in rmsg
	string look_rmsg();

	CString show_rmsg();

private:
	void GET();
	/**
	*@brief the function we choose to deal with the data request like GET
	*@author  ACM2012
	*@param [in] <data> some information we want to get
	*@return void
	*@note
	*与GET方法相同，服务器只返回状态行和头标，
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
	* 2 return "GET HEAD POST PUT DELETE OPTIONS TRACE CONNECT"as Msg
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

