#include "stdafx.h"
#include "Ftpworker.h"

Ftpworker::Ftpworker()
{
	check = new map<string, int>();
	cmd = new map<string, DealWithFunciton>();
	resword = new map<int, string>();
	msg = new FtpMsg();
	sendway = -1;
	connected = false;
	//conn
	(*check)["conn"] = (*check)["CONN"] = 2;
	//people
	(*check)["user"] = (*check)["USER"] = 2;
	(*check)["pass"] = (*check)["PASS"] = 2;
	//tran ways
	(*check)["port"] = (*check)["PORT"] = 1;//send its port to serve
	(*check)["pasv"] = (*check)["PASV"] = 1;//RETR port from server
	//RETR STOR
	(*check)["retr"] = (*check)["RETR"] = 3;//send its port to serve
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

	(*resword)[331] = "user ok, password required";
	(*resword)[332] = "user ok, password wrong";
	(*resword)[330] = "user not found";
	(*resword)[329] = "user not sign in";
	(*resword)[333] = "user login, welcome";
	(*resword)[202] = "operation finished";
	(*resword)[125] = "data ready, starting transfer";
	(*resword)[425] = "can't open data tran";
	(*resword)[452] = "Error writeing";
	(*resword)[346] = "Permission Deny";
	(*resword)[-1] = "error on server";

}

Ftpworker::~Ftpworker()
{
	delete check;
	delete cmd;
	delete msg;
	delete resword;
}

string Ftpworker::getMsg(){
	string data = "";
	if (msg->CMD == "") data += "null";
	else data += msg->CMD;
	data += '\n';
	//data += '\n';
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
	return data;
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

void Ftpworker::checkMsg(){
	msg->word = (*resword)[msg->code];
}

int Ftpworker::dealwith(){
	if (cmd->find(msg->CMD) == cmd->end()){
		return (this->*(*cmd)["error"])();//args
	}
	try{
		return (this->*(*cmd)[msg->CMD])();//args
		
	}
	catch (exception e){
		return (this->*(*cmd)["error"])();//args
	}

}

int Ftpworker::QUIT(){
	//quit
	user.mark = false;//set it not login
	msg->code = 202;
	return 1;
}

int Ftpworker::CONN(){
	return 0;
}

int Ftpworker::USER(){
	if (!user.user_Check(msg->data)){
		msg->code = 330;
	}
	else{
		msg->code = 331;
	}
	return 1;
}
int Ftpworker::PASS(){
	if (!user.ReadConfig()){
		return Error();
	}
	if (user.pass_Check(msg->data)){
		msg->code = 333;
		msg->data = FIO::DirList(user.dir_path);
		user.mark = true;
		return 1;
	}
	else{
		msg->code = 332;
	}
	return 1;
}

int Ftpworker::PORT(){
	sendway = 1;//rev the port to rev a data
	SetPort(STR::string2int(msg->data));
	msg->code = 202;
	msg->data = "port set successfully";
	return 1;
}
int Ftpworker::PASV(){
	sendway = 2;//open a socket when send a file
	msg->code = 202;
	msg->data = "data trans set successfully";
	return 2;
}

int Ftpworker::RETR(){
	msg->SrcPath = user.dir_path+'/'+msg->SrcPath;
	//msg->DesPath = data[2];
	if (!FIO::Exist(msg->SrcPath)){
		msg->code = 418;
		return 1;
	}
	msg->code = 202;
	msg->data = FIO::ReadFile(msg->SrcPath, 1);
	style = false;
	return 3;
}
int Ftpworker::STOR(){
	//msg->SrcPath = data[1];
	msg->DesPath = user.dir_path + '/' + msg->DesPath;
	msg->code = 202;
	FIO::SaveFile(msg->DesPath, &msg->data);
	style = true;
	return 3;
}

int Ftpworker::CWD(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	string path;// 
	if (msg->data == ".") path = user.dir_path;
	else if (msg->data == "..") path = FIO::getFormDir(user.dir_path);
	else path = user.dir_path + "/" + msg->data;
	if (!user.access(path)){
		msg->code = 346;
		return 1;
	}
	if (!FIO::Exist(path)){
		return Error();
	}
	msg->code = 202;
	msg->data = FIO::DirList(path);
	user.dir_path =path;
	return 1;
}
int Ftpworker::MKDIR(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	string path = user.dir_path + "/" + msg->data;
	if (!user.access(path)){
		msg->code = 346;
		return 1;
	}
	if (FIO::Exist(path)){
		msg->code = 417;
		return 1;
	}
	msg->code = 418;
	FIO::CreateDir(path);
	msg->data = FIO::DirList(user.dir_path);
	return 1;
}
int Ftpworker::Error(){
	msg->code = -1;
	return -1;
}
int Ftpworker::ADDUSER(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	if (user.level){
		msg->code = 346;
		return 1;
	}
	People obj;
	if (obj.user_Check(msg->SrcPath)){
		msg->code = 334;
		return 1;
	}
	obj.name = msg->SrcPath;
	obj.level = 3;
	obj.pass = msg->DesPath;
	obj.user_path = "ftp/" + obj.name;
	FIO::CreateDir(obj.user_path);
	obj.SaveCofig();
	return 1;
}
int Ftpworker::DELUSER(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	People obj;
	if (!obj.user_Check(msg->data)){
		msg->code = 330;
		return 1;
	}
	if (!obj.ReadConfig()){
		return Error();
	}
	if (user.Compare(obj.level)){
		FIO::DelDir(obj.user_path);
		msg->code = 335;
	}
	else {
		msg->code = 346;
		return 1;
	}
	return 1;
}

int Ftpworker::RMD(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	if (msg->data == "." || msg->data == ".."){
		msg->code = 346;
		return 1;
	}
	string path = user.dir_path + '/' + msg->data;
	if (FIO::Exist(path)){
		msg->code = 419;
		return 1;
	}
	FIO::DelDir(path);
	FIO::DirList(user.dir_path);
	msg->code = 202;
	msg->data = FIO::DirList(user.dir_path);
	return 1;
}
int Ftpworker::RM(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	string path = user.dir_path +"/"+ msg->data;
	if (!FIO::Exist(path)){
		msg->code = 418;
		return 1;
	}
	FIO::DelFile(path);
	FIO::DirList(user.dir_path);
	msg->code = 202;
	msg->data = FIO::DirList(user.dir_path);
	return 1;
}

int Ftpworker::PWD(){
	if (!user.mark){
		msg->code = 329;
		return 1;
	}
	msg->code = 202;
	msg->data = FIO::DirList(user.dir_path);
	return 1;
}
