#include "stdafx.h"
#include "Ftpworker.h"

Ftpworker::Ftpworker()
{
	check = new map<string, int>();
	cmd = new map<string, DealWithFunciton>();
	sendway = -1;
	connected = false;
	//conn
	(*check)["conn"] = (*check)["CONN"] = 2;
	//people
	(*check)["user"] = (*check)["USER"] = 2;
	(*check)["pass"] = (*check)["PASS"] = 2;
	//tran ways
	(*check)["port"] = (*check)["PORT"] = 2;//send its port to serve
	(*check)["pasv"] = (*check)["PASV"] = 1;//RETR port from server
	//RETR STOR
	(*check)["RETR"] = (*check)["RETR"] = 3;//send its port to serve
	(*check)["stor"] = (*check)["STOR"] = 3;//RETR port from server
	//cd mkdir
	(*check)["cwd"] = (*check)["CWD"] = 2;
	(*check)["mkdir"] = (*check)["MKDIR"] = 2;
	(*check)["rmd"] = (*check)["RMD"] = 2;
	(*check)["rm"] = (*check)["RM"] = 2;
	(*check)["pwd"] = (*check)["PWD"] = 1;//RETR port from server
	//quit
	(*check)["quit"] = (*check)["QUIT"] = 1;//RETR port from server

	(*check)["adduser"] = (*check)["ADDUSER"] = 3;
	(*check)["deluser"] = (*check)["DELUSER"] = 2;

	(*cmd)["conn"] = (*cmd)["CONN"] = &Ftpworker::CONN;
	(*cmd)["pasv"] = (*cmd)["PASV"] = &Ftpworker::PASV;
	(*cmd)["port"] = (*cmd)["PORT"] = &Ftpworker::PORT;
	(*cmd)["user"] = (*cmd)["USER"] = &Ftpworker::USER;
	(*cmd)["pass"] = (*cmd)["PASS"] = &Ftpworker::PASS;
	(*cmd)["cwd"] = (*cmd)["CWD"] = &Ftpworker::CWD;
	(*cmd)["mkdir"] = (*cmd)["MKDIR"] = &Ftpworker::MKDIR;
	(*cmd)["retr"] = (*cmd)["RETR"] = &Ftpworker::RETR;
	(*cmd)["stor"] = (*cmd)["STOR"] = &Ftpworker::STOR;
	(*cmd)["quit"] = (*cmd)["QUIT"] = &Ftpworker::QUIT;
	(*cmd)["error"] = (*cmd)["ERROR"] = &Ftpworker::Error;

	(*cmd)["adduser"] = (*cmd)["ADDUSER"] = &Ftpworker::ADDUSER;
	(*cmd)["deluser"] = (*cmd)["DELUSER"] = &Ftpworker::DELUSER;
	(*cmd)["rmd"] = (*cmd)["RMD"] = &Ftpworker::RMD;
	(*cmd)["rm"] = (*cmd)["RM"] = &Ftpworker::RM;
	(*cmd)["pwd"] = (*cmd)["PWD"] = &Ftpworker::PWD;
}

Ftpworker::~Ftpworker()
{
	delete check;
	delete cmd;
	delete msg;
}

void Ftpworker::Dealfile(int code){
	if (code == 2){
		if (style){
			FIO::SaveFile(msg->DesPath, &msg->data);
		}
	}
}

string Ftpworker::getMsg(){
	string data = "";
	if (msg->CMD == "") data += "null";
	else data += msg->CMD;
	data += '\n';
	if (msg->SrcPath == "") data += "null";
	else data += msg->SrcPath;
	data += '\n';
	if (msg->DesPath == "") data += "null";
	else data += msg->DesPath;
	data += '\n';
	data += STR::int2string(0);
	data += '\n';
	if (msg->word == "") data += "null";
	else data += msg->word;
	data += '\n';
	if (msg->data == "") data += "null";
	else data += msg->data;
	//data += '\n';
	return data;
}

CString Ftpworker::show_msg(){
	CString look=_T("\r\n");
	look += STR::S2CS(msg->CMD + "  ->  "+msg->word)+_T("\r\n");
	look += _T("the data you send is  ")+STR::S2CS(msg->data);
	return look;
}

CString Ftpworker::show_msg(string save){
	CString look = _T("\r\n");
	look += STR::S2CS(msg->CMD + "  ->  " + msg->word) + _T("\r\n");
	if(save!=msg->data)
		look += _T("the data you get is  \r\n") + STR::S2CS(msg->data);
	else if (msg->data=="")
		look += _T("the data you send is null");
	else
		look += _T("the data you send is   ") + STR::S2CS(save);
	return look;
}

void Ftpworker::setMsg(string data){
	msg = new FtpMsg();
	vector<string>d;
	STR::Split(data, &d, '\n');
	if (d[0] == "null"){
		msg->CMD = "";
	}
	else msg->CMD = d[0];
	if (d[1] == "null"){
		msg->SrcPath = "";
	}
	else msg->SrcPath = d[1];
	if (d[2] == "null"){
		msg->DesPath = "";
	}
	else msg->DesPath = d[2];
	msg->code = STR::string2int(d[3]);
	if (d[4] == "null"){
		msg->word = "";
	}
	else msg->word = d[4];
	if (d[5] == "null"){
		msg->data = "";
	}
	else msg->data = d[5];
	for (int i = 6; i < d.size(); i++){
		msg->data + "\n" + d[i];
	}
}

int Ftpworker::div(string Msg, char split){
	data.clear();
	msg = new FtpMsg();
	STR::Split(Msg, &data, split);
	msg->CMD = data[0];
	if (data.size()<(*check)[data[0]]){
		return -1; //arcg miss
	}
	return 1;
}
int Ftpworker::Analy(){
	//according to the ans , we RETR diff way
	return 1;
}
int Ftpworker::dealwith(){
	if (cmd->find(data[0]) == cmd->end()){
		return (this->*(*cmd)["error"])();//args
	}
	try{
		return (this->*(*cmd)[data[0]])();//args
	}
	catch (exception e){
		return (this->*(*cmd)["error"])();//args
	}
	/*
	if (msg->CMD == "pasv" || msg->CMD == "PASV"){
		sendway = 1;
	}
	if (msg->CMD == "port" || msg->CMD == "PORT"){
		sendway = 2;
	}*/
	
}

int Ftpworker::QUIT(){
	//quit
	return 1;
}
int Ftpworker::CONN(){
	//conn
	ip = data[1];
	return 0;
}

int Ftpworker::USER(){
	msg->data = data[1];
	return 1;
}
int Ftpworker::PASS(){
	msg->data = data[1];
	return 1;
}

int Ftpworker::PORT(){
	sendway = 1;//rev the port to rev a data
	msg->data = data[1];
	RevPort = STR::string2int(data[1]);
	return 3;
}
int Ftpworker::PASV(){
	sendway = 2;//open a socket when send a file
	return 3;
}

int Ftpworker::RETR(){
	msg->SrcPath = data[1];
	msg->DesPath = data[2];
	
	style = true;
	return 2;
}

int Ftpworker::STOR(){
	msg->SrcPath = data[1];
	msg->DesPath = data[2];
	msg->data = FIO::ReadFile(msg->SrcPath, 1);
	style = false;
	return 2;
}

int Ftpworker::CWD(){
	msg->data = data[1];
	return 1;
}
int Ftpworker::MKDIR(){
	msg->data = data[1];
	return 1;
}
int Ftpworker::Error(){
	return -1;
}


int Ftpworker::ADDUSER(){
	msg->SrcPath = data[1];
	msg->DesPath = data[2];
	return 1;
}

int Ftpworker::DELUSER(){
	msg->data = data[1];
	return 1;
}

int Ftpworker::RMD(){
	msg->data = data[1];
	return 1;
}

int Ftpworker::RM(){
	msg->data = data[1];
	return 1;
}

int Ftpworker::PWD(){
	return 1;
}
