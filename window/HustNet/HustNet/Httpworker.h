#pragma once
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "Tools.h"

using namespace Tools;
using namespace std;

/**/
typedef unsigned short version;
typedef unsigned int http_error_code;
typedef unsigned int http_code;
typedef map<string, int> checkarg;
typedef string IP;


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

struct HttpRMsg{
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
	http_error_code div(string Msg, char split);
	void Make();
	http_error_code analy();

	bool setMsg(string rec);

	string getMsg(){
		return look_msg();
	}
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
};

