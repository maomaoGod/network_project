// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 HTTPWORKER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// HTTPWORKER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#pragma once
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "Tools.h"

using namespace Tools;
using namespace std;

typedef string version;
typedef string http_code;
typedef map<string, int> checkarg;
typedef string IP;
#define MarkHttp "\r\n"
/**/
typedef string version;
typedef string http_code;
typedef map<string, int> checkarg;
typedef string IP;
#define MarkHttp "\r\n"

struct Httpcookie{
	int num;
	string history;
	int RandCook(){
		num = 1000 + rand()%9000;
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
		STR::Split(r, &d, '\n');
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

struct HttpMsg{
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
};

struct HttpRMsg{
	struct{
		version no;
		http_code code;
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

#ifdef HTTPWORKER_EXPORTS
#define HTTPWORKER_API __declspec(dllexport)
#else
#define HTTPWORKER_API __declspec(dllimport)
#endif

// 此类是从 Httpworker.dll 导出的
class HTTPWORKER_API Httpworker {
public:
	Httpworker();
	// TODO: 在此添加您的方法。
	~Httpworker();
	int div(string Msg, char split);
	void Make();
	int analy();

	int setMsg(string rec);
	string getMsg();

	string gethost(){
		return host;
	}
	IP getIP(){
		return ip;
	}
	void setIP(IP ip){
		this->ip = ip;
	}
	bool IPcheck(){
		int i;
		for (i = 0; i < host.length(); i++){
			if (host[i] == '.' || (host[i] <= '9'&&host[i] >= '0')){
				continue;
			}
			return false;
		}
		return true;
	}
	void setdata(string msg){
		rmsg->data = msg;
	}
	string getdata(){
		return rmsg->data;
	}

	void setPort(int port){
		Port = port;
	}
	int getPort(){
		if (Port < 1024){
			Port = 80;
		}
		return Port;
	}

	string look_msg();
	string look_rmsg();

	CString show_rmsg();
private:
	HttpMsg *msg;
	HttpRMsg *rmsg;
	checkarg *alg;
	string host;
	IP ip;
	vector<string> data;
	int Port;
};

extern HTTPWORKER_API int nHttpworker;

HTTPWORKER_API int fnHttpworker(void);

// C API to the user
#define FUNCHTTP extern "C" __declspec(dllexport) 

FUNCHTTP void *NewHttptr(){
	return (void *)(new Httpworker());
}
FUNCHTTP int Httpdiv(void *ele, string Msg, char split){
	Httpworker *tmp = (Httpworker *)ele;
	int tt = tmp->div(Msg, split);
	return tt;
}
FUNCHTTP void setPort(void *ele, int port){
	Httpworker *tmp = (Httpworker *)ele;
	tmp->setPort(port);
}
FUNCHTTP int getPort(void*ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->getPort();
}
FUNCHTTP void Make(void *ele){
	Httpworker *tmp = (Httpworker *)ele;
	tmp->Make();
}
FUNCHTTP int analy(void *ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->analy();
}
FUNCHTTP int setMsg(void *ele, string rec){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->setMsg(rec);
}
FUNCHTTP string getMsg(void *ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->getMsg();
}
FUNCHTTP string gethost(void *ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->gethost();
}
FUNCHTTP IP getIP(void *ele){
	return ((Httpworker *)ele)->getIP();
}
//FUNCHTTP
FUNCHTTP void setIP(void* ele, IP ip){
	Httpworker *tmp = (Httpworker *)ele;
	tmp->setIP(ip);
}
FUNCHTTP bool IPcheck(void* ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->IPcheck();
}
FUNCHTTP string look_msg(void *ele){
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->look_msg();
}
FUNCHTTP void setdata(void* ele, string msg)
{
	Httpworker *tmp = (Httpworker *)ele;
	tmp->setdata(msg);
}
FUNCHTTP string getdata(void* ele)
{
	Httpworker *tmp = (Httpworker *)ele;
	return tmp->getdata();
}
