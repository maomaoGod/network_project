#pragma once
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "Tools.h"

using namespace Tools;
using namespace std;

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


class Httpworker
{
public:
	Httpworker();
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

