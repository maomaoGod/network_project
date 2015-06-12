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

int Httpworker::div(string Msg, char split){
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
	if ((alg->find(data[0])) == alg->end()){
		return -1;
	}
	if(data.size() < (*alg)[data[0]]) 
		return -1;// lost argc
	data[0] = STR::Upper(data[0]);
	i = data[1].find('/');
	this->host = data[1].substr(0, i);
	//data[1] = data[1].substr(i + 1);
	if (i == -1 || (this->host+'/'==data[1])){
		data[1] = "/";
	}
	else data[1] = data[1].substr(i);
	return 1;//return success 
}

void Httpworker::Make(){
	if(msg) delete msg;
	msg = new HttpMsg();
	msg->method = data[0];
	msg->path = data[1];
	msg->no = "HTTP/1.1";
	msg->host = this->host;
	msg->user = "HUST ACM2012 Virtual Terminal - v1.0";
	msg->language = "zh-CN";
	msg->connect = "close";
	msg->accept = "text/html";
	msg->accept_enconding = "gzip,deflate,sdch";
	string path;
	int len = msg->path.length();
	if (msg->path[len - 1] == '/') 
		path = "cache/" + msg->path + "index.html";
	else path = "cache/" + msg->path;
	if (FIO::Exist(path)){
		msg->if_modified_since = FIO::ReadFullFile(path+".cache");
		int len = msg->if_modified_since.length();
		msg->if_modified_since = msg->if_modified_since.substr(0,len-4);
	}
	else msg->if_modified_since = "";
	if (Conntion) msg->connect = "keep-alive";
	//at last
	
	msg->cookie.num = -1;
	/*
	if (!msg->cookie.readcook_host(getIP())){
		msg->cookie.num = -1;
	}*/
	for (int i = 2; i < data.size(); i++)
		msg->data += data[i];
	msg->length = msg->data.length();
}

int Httpworker::analy(){
	if (rmsg->no != msg->no){
		return -1;// no version
	}
	//not much analy 
	//web cache is right
	string path;
	int len = msg->path.length();
	if (msg->path[len-1]=='/') 
		path = "cache/" + msg->path+"index.html";
	else path = "cache/" + msg->path;
	if (rmsg->code == "304"){
		rmsg->data = FIO::ReadFullFile(path);
	}
	else if (rmsg->code == "200" && (msg->method=="get"||msg->method=="GET")){
		//new cache page
		if (FIO::Exist(path)){
			FIO::DelFile(path);
			FIO::DelFile(path + ".cache");
		}
		FIO::SaveFile(path, &rmsg->data);
		FIO::SaveFile(path + ".cache", &rmsg->last_modified);
	}
	//rmsg->cookie.create_host(ip);
	return 1;//success
}

string Httpworker::look_rmsg(){
	string look;
	string a, b, c;
	c = STR::int2string(rmsg->length);
	look = a + "\n" + b + "\n" + rmsg->word + " " + rmsg->connect + "\n";
	look += rmsg->date + "\n" + rmsg->server + "\n" + rmsg->last_modified + "\n" + c + "\n";
	look += rmsg->type[0] + "\n" + "cookie\n" + rmsg->data;
	return look;
}
//return all string in rmsg
string Httpworker::look_msg(){
	string Look;
	string a;
	Look = msg->method + " " + msg->path + " " + msg->no + "\n";
	Look += "host: "+msg->host + "\n" +" User-Agent: " +msg->user + "\n";
	Look += msg->language + "\n" + msg->connect + "\n";
	Look += msg->if_modified_since + "\n";
	//;
	if (msg->cookie.num){
		Look += STR::int2string(msg->cookie.num) + '\n';
	}
	else Look += "cookie\n";
	Look += msg->data;
	return Look;
}

CString Httpworker::show_rmsg(){
	CString look = _T("\r\n");
	string a, b, c;
	c = STR::int2string(rmsg->length);
	look += _T("Version : ") + STR::S2CS(a) + _T("\r\n");
	look += _T("Error_cpde : ") + STR::S2CS(b) + _T("  ") + STR::S2CS(rmsg->word) + _T("\r\n") + _T("Connection :") + STR::S2CS(rmsg->connect) + _T("\r\n");
	look += _T("Date : ") + STR::S2CS(rmsg->date) + _T("\r\n") + _T("Server : ") + STR::S2CS(rmsg->server) + _T("\r\n");
	look += _T("Last_modified : ") + STR::S2CS(rmsg->last_modified) + _T("\r\n") + _T("Length : ") + STR::S2CS(c) + _T("\r\n");
	look += _T("Type : ") + STR::S2CS(rmsg->type[0]) + _T("\r\n") + _T("Cookie : ") + STR::S2CS(STR::int2string(rmsg->cookie.num)) + _T("\r\n");
	look +=  _T("Data : ") + STR::S2CS(rmsg->data) + _T("\r\n");
	return look;
}

int Httpworker::setMsg(string rec){
	vector<string> d;
	STR::Split(rec, &d, "\r\n");
	rmsg = new HttpRMsg();
	int i;
	//the begining must be exist
	rmsg->length = 0;
	rmsg->setCode(d[0]);
	for (i = 1; i < d.size(); i++){
		if (d[i] == "") break;
		else rmsg->findHead(d[i]);
	}
	rmsg->data = "";
	if (rmsg->length == 0) return -1;
	i++;
	for (; i < d.size(); i++){
		rmsg->data += (d[i]+"\r\n");
	}
	int len = rmsg->data.length();
	rmsg->data = rmsg->data.substr(0, len-2);
	return rmsg->length - len +2;
}

string Httpworker::getMsg(){
	string look = "";
	look += (msg->method + ' '+msg->path+' '+msg->no) + MarkHttp;
	look += "Host: " + msg->host + MarkHttp;
	//look += "Accept: "+ msg->accept+ MarkHttp;
	//look += "Accept-Encoding: "+msg->accept_enconding+MarkHttp;
	look += "Accept-Language: "+msg->language+MarkHttp;
	look += "Connection: " + msg->connect + MarkHttp;
	msg->cookie.num = -1;//not open cookie
	if (msg->cookie.num != -1){
		look += "Cookie: " + STR::int2string(msg->cookie.num) + MarkHttp;
	}
	if (msg->length != 0){
		look += "Content-Length: " + STR::int2string(msg->length) + MarkHttp;
	}
	if (msg->if_modified_since!="")
		look += "If_modified_since: " + msg->if_modified_since + MarkHttp;
	if(getPort()!= 80)
		look += "User-Agent: "+msg->user+MarkHttp;
	look += MarkHttp;
	//data
	look += msg->data;
	return look;
}

/*
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
rmsg->type.push_back(d[8]);
vector<string> c;
STR::Split(d[9],&c,'|');
rmsg->cookie.num = STR::string2int(c[0]);
rmsg->cookie.history = c[1];
rmsg->cookie.create_host(getIP());
rmsg->data = d[10];
for (int i = 11; i < d.size(); i++)
rmsg->data += ('\n'+d[i]);
return true;*/
