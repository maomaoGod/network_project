#include "stdafx.h"
#include "Httpworker.h"

#define Conntion 0

Httpworker::Httpworker()
{
	//nothing
	alg = new checkarg();
	fun = new map<string, DealWithFunciton>();
	word = new resword();
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

	//set fun
	(*fun)["GET"] = (*fun)["get"] = &Httpworker::GET;
	(*fun)["HEAD"] = (*fun)["head"] = &Httpworker::HEAD;
	(*fun)["POST"] = (*fun)["post"] = &Httpworker::POST;
	(*fun)["PUT"] = (*fun)["put"] = &Httpworker::PUT;
	(*fun)["DELETE"] = (*fun)["delete"] = &Httpworker::DELETEFILE;
	(*fun)["OPTIONS"] = (*fun)["options"] = &Httpworker::OPTIONS;
	(*fun)["TRACE"] = (*fun)["trace"] = &Httpworker::TRACE_HTTP;
	(*fun)["CONNECT"] = (*fun)["connect"] = &Httpworker::CONNECT;
	//bad res
	(*fun)["BAD"] = (*fun)["bad"] = &Httpworker::BAD_RES;
	//int error
	(*fun)["ERROR"] = (*fun)["error"] = &Httpworker::ERROR_HTTP;

	(*word)["200"] = "OK";
	(*word)["301"] = "MOVE Permanently";
	(*word)["304"] = "NOT Modified";
	(*word)["400"] = "BAD REQUEST";
	(*word)["404"] = "NOT FOUND";
	(*word)["505"] = "HTTP VERSION NOT SUPPORTED";
	(*word)["-1"] = "Server crashed";
}

Httpworker::~Httpworker()
{
	delete msg;
	delete rmsg;
	delete alg;
	delete fun;
	delete word;
	host = "";
}

void Httpworker::Make(){
	if (fun->find(rmsg->method) == fun->end()){
		(this->*(*fun)["bad"])();//args
		return;
	}
	try{
		(this->*(*fun)[rmsg->method])();//args
	}
	catch (exception e){
		(this->*(*fun)["error"])();//args
		return;
	}
}

int Httpworker::analy(){
	//set msg;
	msg = new HttpMsg();
	msg->connect = "Closed";//rmsg->connect;
	msg->no = rmsg->no;
	msg->length = 0;
	msg->date = Tools::Timer::GetCurtime();
	msg->server = "HUST ACM2012 Virtual Terminal - v1.0";
	if (FIO::Exist(rmsg->path))
		msg->last_modified = Tools::Timer::FilelastChange(rmsg->path);
	else
		msg->last_modified = "";
	//cookie file produce
	/*
	if (rmsg->cookie.num == -1){
		int x = msg->cookie.RandCook();
		while (msg->cookie.findcookie()){
			x = msg->cookie.RandCook();
		}
		msg->cookie.history = rmsg->method + "->" + msg->date;
		msg->cookie.create(x);
	}
	else{
		msg->cookie.readcook(rmsg->cookie.num);
		msg->cookie.history += ('*' + rmsg->method + "->" + msg->date);
		msg->cookie.create(msg->cookie.num);
	}*/
	return 1;//success
}

void Httpworker::check(){
	if (msg->type.size()==0) msg->type.push_back("text/html");
	if (FIO::Exist(rmsg->path)&&msg->last_modified=="")
		msg->last_modified = Tools::Timer::FilelastChange(rmsg->path);
	if (msg->last_modified == "") msg->last_modified = "";
	msg->length = msg->data.length();
}

CString Httpworker::show_rmsg(){
	CString look = _T("\r\n");
	look += _T("Method : ") + STR::S2CS(rmsg->method) + _T("\r\n");
	look += _T("path : ") + STR::S2CS(rmsg->path) + _T("\r\n");
	look += _T("Version : ") + STR::S2CS(rmsg->no) + _T("\r\n");
	look += _T("Host : ") + STR::S2CS(rmsg->host) + _T("\r\n");
	look += _T("User : ") + STR::S2CS(rmsg->user) + _T("\r\n");
	look += _T("Language : ") + STR::S2CS(rmsg->language) + _T("\r\n");
	look += _T("Connection :") + STR::S2CS(rmsg->connect) + _T("\r\n");
	look += _T("If_modified_since : ") + STR::S2CS(rmsg->if_modified_since) + _T("\r\n");
	look += _T("cookie : ")+ STR::S2CS(STR::int2string(rmsg->cookie.num))+_T("\r\n");
    return look;
}

//return all string in msg
string Httpworker::look_msg(){
	check();
	string look;
	look = msg->no + ' ' + msg->code + ' ' + msg->word + MarkHttp;
	look += "Date: " + msg->date + MarkHttp;
	look += "Content-Type: " + msg->type[0] + MarkHttp;
	look += "Content-Length: " + STR::int2string(msg->length) + MarkHttp;
	look += "Last-Modified: " + msg->last_modified + MarkHttp;
	look += "Vary: " + msg->vary + MarkHttp;
	look += "Set-Cookie: " + STR::int2string(msg->cookie.num) + '|'+msg->cookie.history + MarkHttp;
	look += "Server: " + msg->server + MarkHttp;
	look += "Accept-Ranges: " + msg->accapt_ranges + MarkHttp;
	look += MarkHttp;//head finish
	look += msg->data;
	return look;
}
//return all string in rmsg
string Httpworker::look_rmsg(){
	string Look;
	Look = rmsg->method + "\n" + rmsg->path + "\n" + rmsg->no + "\n" ;
	Look += rmsg->host + "\n" + rmsg->user + "\n";
	Look += rmsg->language + "\n" + rmsg->connect + "\n";
	Look += rmsg->if_modified_since + "\n";
	Look += STR::int2string(rmsg->cookie.num) + '|' + rmsg->cookie.history;
	Look += rmsg->data;
	return Look;
}

int Httpworker::setMsg(string rec){
	vector<string> d;
	rmsg = new HttpRMsg();
	int i;
	STR::Split(rec, &d, "\r\n");
	rmsg->length = 0;
	rmsg->setCode(d[0]);
	for (i = 1; i < d.size(); i++){
		if (d[i] == "") break;
		else rmsg->findHead(d[i]);
	}
	int len;
	len = rmsg->path.length();
	rmsg->path = rmsg->path.substr(1, len - 1);
	if (rmsg->length == 0) return -1;
	rmsg->data = "";
	i++;
	for (; i < d.size(); i++){
		rmsg->data += (d[i] + "\r\n");
	}
	len = rmsg->data.length();
	rmsg->data = rmsg->data.substr(0, len - 2);
	return rmsg->length - len + 2;
}

string Httpworker::getMsg(){
	check();
	string look;
	look = msg->no + ' ' + msg->code + ' ' + msg->word + MarkHttp;
	look += "Date: " + msg->date + MarkHttp;
	look += "Content-Type: " + msg->type[0] + MarkHttp;
	look += "Content-Length: " + STR::int2string(msg->length) + MarkHttp;
	look += "Connect: " + msg->connect + MarkHttp;
	look += "Last-Modified: " + msg->last_modified + MarkHttp;
	look += "Vary: " + msg->vary + MarkHttp;
	//look += "Set-Cookie: " + STR::int2string(msg->cookie.num) + '|' + msg->cookie.history + MarkHttp;
	look += "Server: " + msg->server + MarkHttp;
	look += "Accept-Ranges: " + msg->accapt_ranges + MarkHttp;
	look += MarkHttp;//head finish
	look += msg->data;
	return look;
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
/*

int length;
string data;
*/

void Httpworker::GET(){
	// not exist
	if (!Tools::FIO::Exist(rmsg->path)){
		msg->code = "404";
		msg->word = (*word)[msg->code];
		return;
	}
	// web cache
	if (rmsg->if_modified_since != ""){
		//condtional get
		if (msg->last_modified == rmsg->if_modified_since){
			msg->code = "304";
			msg->word = (*word)[msg->code];
			msg->length = FIO::file_size(rmsg->path);
			return;
		}
	}
	// 200
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->type.push_back("html");
	msg->data = FIO::ReadFullFile(rmsg->path); //Tools::FIO::ReadFile(rmsg->path);
	return;
}
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
void Httpworker::HEAD(){
	if (!Tools::FIO::Exist(rmsg->path)){
		msg->code = "404";
		msg->word = (*word)[msg->code];
		return;
	}
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->type.push_back("html");
	msg->data = "path in serve: " + rmsg->path;
}
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
void Httpworker::POST(){
	if (!Tools::FIO::Exist(rmsg->path)){
		msg->code = "404";
		msg->word = (*word)[msg->code];
		return;
	}
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->data = FIO::ReadFullFile(rmsg->path) + rmsg->data;
	FIO::SaveFile(rmsg->path, &msg->data);
}
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
void Httpworker::PUT(){
	if (Tools::FIO::Exist(rmsg->path)){
		msg->code = "200";
		msg->word = (*word)[msg->code];
		msg->data = "The page is corved.";
		FIO::DelFile(rmsg->path);
	}
	else msg->data = "The page is created." ;
	msg->code = "200";
	msg->word = (*word)[msg->code];
	FIO::SaveFile(rmsg->path, &rmsg->data);
	msg->type.push_back("html");
}
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
void Httpworker::DELETEFILE(){
	if (!Tools::FIO::Exist(rmsg->path)){
		msg->code = "404";
		msg->word = (*word)[msg->code];
		return;
	}
	msg->code = "200";
	msg->word = (*word)[msg->code];
	FIO::DelFile(rmsg->path);
	msg->type.push_back("html");
	msg->data = "Successfully delete";
}
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
void Httpworker::OPTIONS(){
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->type.push_back("html");
	msg->data = "";
	msg->data += "GET : ";
	msg->data += "Retrieve a simple request URL identifying the resources.";
	msg->data += "\r\n";

	msg->data += "HEAD : ";
	msg->data += "Same as the GET method, the server returns only the status line and head.";
	msg->data += "\r\n";

	msg->data += "POST : ";
	msg->data += "The server accepts the request of the data that are written to the client output stream.";
	msg->data += "\r\n";

	msg->data += "PUT : ";
	msg->data += "The server save request data as the specified URL request of new content.";
	msg->data += "\r\n";

	msg->data += "DELETE : ";
	msg->data += "delete the file on the server according to URL";
	msg->data += "\r\n";

	msg->data += "OPTIONS : ";
	msg->data += "The request of the information about request methods of the server supports.";
	msg->data += "\r\n";

	msg->data += "CONNECT : ";
	msg->data += "The method, which has documented but not implented currently, reserved for the tunnel processing.";
	msg->data += "\r\n";

}
/**
*@brief don't Know what's it meaning
*@author  ACM2012
*@param [in] <data> some information we want to get
*@return void
*@note
*Web服务器反馈Http请求和其头标的请求,Map it to 7
*/
void Httpworker::TRACE_HTTP(){
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->type.push_back("html");
	//msg->data =  "0"; get all cookies
}
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
void Httpworker::CONNECT(){
	msg->code = "200";
	msg->word = (*word)[msg->code];
	msg->type.push_back("html");
	msg->data = "The request from client has been received";
}

void Httpworker::BAD_RES(){
	msg->code = "400";
	msg->word = (*word)[msg->code];
}

void Httpworker::ERROR_HTTP(){
	msg->code = "-1";
	msg->word = (*word)[msg->code];
}
