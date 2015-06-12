#pragma once

#include "string"
#include "map"
#include "Tools.h"

using namespace Tools;
using namespace std;

#define data_port 7601
#define ftp_port 7600

typedef string Datatran;
typedef string IP; 


struct People{
	string name;
	string pass;
	int level;
	string dir_path;
	string user_path;
	
	void SaveCofig(){
		string path = "ftp/" + name + "/.config";
		if (FIO::Exist(path.c_str())) FIO::DelFile(path.c_str());
		string data = pass + "\n" + STR::int2string(level);
		FIO::SaveFile(path.c_str(), &data);
	}
	
	bool ReadConfig(){
		string path = "ftp/" + name + "/.config";
		user_path = "ftp/" + name;
		if (!FIO::Exist(path.c_str())){
			return false;
		}
		string data = FIO::ReadFile(path, 1);
		vector<string> d;
		STR::Split(data, &d, '\n');
		pass = d[0];
		level = STR::string2int(d[1]);
		return true;
	}
	
	int getLevel(){
		return level;
	}
	
	bool access(string path){
		if (!getLevel()) return true;
		//still in user_path
		if (path.find(user_path) != string::npos) 
			return true;
		return false;
	}
	
	bool user_Check(string name){
		if (FIO::Exist("ftp/" + name)){
			this->name = name;
			return true;
		}
		return false;
	}
	
	bool pass_Check(string pass){
		if (this->pass == pass){
			//init the user path
			dir_path = user_path;
			return true;
		}
		return false;
	}

};

struct FtpMsg{
	string CMD;
	string data;
	string SrcPath;
	string DesPath;
	int code;
	string word;
};

class Ftpworker
{
public:
	Ftpworker();
	~Ftpworker();
	int div(string Msg, char split);
	int Analy();
	int dealwith();
	string getMsg();
	void setMsg(string rec);
	CString show_msg();
	CString show_msg(string data);
	void Dealfile(int code);
	string getSrc(){
		return msg->SrcPath;
	}
	string getDes(){
		return msg->DesPath;
	}

	IP getIP(){
		return ip;
	}
	Datatran getData(){
		return datatran;
	}
	void setData(string rec){
		datatran = rec;
	}
	string getSmallData(){
		return msg->data;
	}
	bool getStyle(){
		return style;
	}
	void SetPort(int port){
		RevPort = port;
	}
	int GetPort(){
		return RevPort;
	}
	bool connected;
	int getWay(){
		return sendway;
	}
private:
	People user;
	FtpMsg *msg;
	bool style;
	IP ip;
	int RevPort;
	int sendway;
	vector<string> data;
	map<string, int> *check;
	typedef int(Ftpworker::*DealWithFunciton)();
	map<string, DealWithFunciton> *cmd;
	Datatran datatran;
	int QUIT();
	int CONN();
	//people
	int USER();
	int PASS();
	//tran ways
	int PORT();
	int PASV();//get port from server
	//get put
	int RETR();//send its port to serve
	int STOR();//get port from server
	//cd mkdir
	int CWD();
	int MKDIR();
	int PWD();

	int Error();

	int ADDUSER();
	int DELUSER();

	int RMD();
	int RM();
};

