#include "stdafx.h"
#include "Httpworker.h"
#define Conntion 0

Httpworker::Httpworker()
{
	//nothing
	srand(time(NULL));
	alg = new checkarg();
	(*alg)["get"] = (*alg)["GET"] = 2;
	(*alg)["post"] = (*alg)["POST"] = 3;
	(*alg)["put"] = (*alg)["PUT"] = 3;
	(*alg)["head"] = (*alg)["HEAD"] = 2;
	(*alg)["delete"] = (*alg)["DELETE"] = 2;
	(*alg)["options"] = (*alg)["OPTIONS"] = 2;
	(*alg)["trace"] = (*alg)["TRACE"] = 2;
	(*alg)["connect"] = (*alg)["CONNECT"] = 2;

	//connect
	(*alg)["close"] = 0;
	(*alg)["keep-alive"] = 1;
}

Httpworker::~Httpworker()
{
	delete msg;
	delete rmsg;
	delete alg;
	host = "";
	data.clear();
}

http_error_code Httpworker::div(string Msg, char split){
	data.clear();
	if (Msg.size() == 0){
		return -1;// none msg error
	}
	//div
	int i, j;
	int len = Msg.length();
	for (i = 0; i < len; i++){
		for (j = i; j < len; j++){
			if (Msg[j] == split) break;
		}
		if (j>i) data.push_back(Msg.substr(i, j - i));
		i = j;
	}
	//check argc
	if(data.size() < (*alg)[data[0]]) 
		return 0;// lost argc
	//div url -> host + path
	// that means that we put host to host, put path to data[1]
	i = data[1].find('/');
	this->host = data[1].substr(0, i);
	data[1] = data[1].substr(i + 1);
	return 1;//return success 
}

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
string data;
*/

void Httpworker::Make(){
	if(msg) delete msg;
	msg = new HttpMsg();
	msg->method = data[0];
	msg->path = data[1];
	msg->no = 11;
	msg->host = this->host;
	msg->user = "HUST ACM2012 Virtual Terminal - v1.0";
	msg->language = "chinese";
	msg->connect = "close";
	msg->if_modified_since = "null";
	string path = "cache/" + msg->path;
	if (FIO::Exist(path)){
		msg->if_modified_since = FIO::ReadFile(path+".cache", 0);
	}
	if (Conntion) msg->connect = "keep-alive";
	//at last
	msg->cookie.num = 0;
	if (!msg->cookie.readcook_host(getIP())){
		msg->cookie.num = -1;
	}
	for (int i = 2; i < data.size(); i++)
		msg->data += data[i];
	if (msg->data == "") msg->data = "null";
	msg->options = "null";
}

http_error_code Httpworker::analy(){
	if (rmsg->no != msg->no){
		return -1;// no version
	}
	//not much analy 
	//web cache is right
	string path = "cache/" + msg->path;
	if (rmsg->code == 304){
		rmsg->data = FIO::ReadFile(path, 0);
	}
	else if (rmsg->code == 200 && (msg->method=="get"||msg->method=="GET")){
		//new cache page
		if (FIO::Exist(path)){
			FIO::DelFile(path);
			FIO::DelFile(path + ".cache");
		}
		FIO::SaveFile(path, &rmsg->data);
		FIO::SaveFile(path + ".cache", &rmsg->last_modified);
	}
	rmsg->cookie.create_host(ip);
	return rmsg->code;
}

string Httpworker::look_rmsg(){
	string look;
	string a, b, c;
	a = STR::int2string(rmsg->no);
	b = STR::int2string(rmsg->code);
	c = STR::int2string(rmsg->length);
	look = a + "\n" + b + "\n" + rmsg->word + "\n" + rmsg->connect + "\n";
	look += rmsg->date + "\n" + rmsg->server + "\n" + rmsg->last_modified + "\n" + c + "\n";
	look += rmsg->type + "\n" + "cookie\n" + rmsg->data;
	return look;
}
//return all string in rmsg
string Httpworker::look_msg(){
	string Look;
	string a;
	a = STR::int2string(msg->no);
	Look = msg->method + "\n" + msg->path + "\n" + a + "\n";
	Look += msg->host + "\n" + msg->user + "\n";
	Look += msg->language + "\n" + msg->connect + "\n";
	Look += msg->if_modified_since + "\n";
	//;
	if (msg->cookie.num){
		Look += STR::int2string(msg->cookie.num) + '\n';
	}
	else Look += "cookie\n";
	Look += msg->options + "\n";
	Look += msg->data;
	return Look;
}


CString Httpworker::show_rmsg(){
	CString look = _T("\r\n");
	string a, b, c;
	a = STR::int2string(rmsg->no);
	b = STR::int2string(rmsg->code);
	c = STR::int2string(rmsg->length);
	look += _T("Version : ") + STR::S2CS(a) + _T("\r\n");
	look += _T("Error_cpde : ") + STR::S2CS(b) + _T("  ") + STR::S2CS(rmsg->word) + _T("\r\n") + _T("Connection :") + STR::S2CS(rmsg->connect) + _T("\r\n");
	look += _T("Date : ") + STR::S2CS(rmsg->date) + _T("\r\n") + _T("Server : ") + STR::S2CS(rmsg->server) + _T("\r\n");
	look += _T("Last_modified : ") + STR::S2CS(rmsg->last_modified) + _T("\r\n") + _T("Length : ") + STR::S2CS(c) + _T("\r\n");
	look += _T("Type : ") + STR::S2CS(rmsg->type) + _T("\r\n") + _T("Cookie : ") + STR::S2CS(STR::int2string(rmsg->cookie.num)) + _T("\r\n");
	look +=  _T("Data : ") + STR::S2CS(rmsg->data) + _T("\r\n");
	return look;
}


bool Httpworker::setMsg(string rec){
	vector<string> d;
	rmsg = new HttpRMsg();
	STR::Split(rec, &d, '\n');
	if (d.size() < 11) return false;
	rmsg->no = STR::string2int(d[0]);
	rmsg->code = STR::string2int(d[1]);
	rmsg->word = d[2];
	//host
	rmsg->connect = d[3];
	rmsg->date = d[4];
	rmsg->server = d[5];
	rmsg->last_modified = d[6];
	rmsg->length = STR::string2int(d[7]);
	rmsg->type = d[8];
	vector<string> c;
	STR::Split(d[9],&c,'|');
	rmsg->cookie.num = STR::string2int(c[0]);
	rmsg->cookie.history = c[1];
	rmsg->cookie.create_host(getIP());
	rmsg->data = d[10];
	for (int i = 11; i < d.size(); i++)
		rmsg->data += ('\n'+d[i]);
	return true;
}
