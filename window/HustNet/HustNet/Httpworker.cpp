/**
*@file
*@brief Http协议客户端的实现
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
* 实现http协议客户端的功能，实现cookie，
* 设计请求报文和应答报文的结构，
*/
#include "stdafx.h"
#include "Httpworker.h"
#define Conntion 0
/**
*@brief Httpworker
*@author  ACM2012
*@return void
*@note
* Httpworker的构造函数的实现
*/
Httpworker::Httpworker()
{
	//nothing
	srand(time(NULL));
	alg = new checkarg();
	///<Http协议的命令
	(*alg)["get"] = (*alg)["GET"] = 2;
	(*alg)["post"] = (*alg)["POST"] = 3;
	(*alg)["put"] = (*alg)["PUT"] = 3;
	(*alg)["head"] = (*alg)["HEAD"] = 2;
	(*alg)["delete"] = (*alg)["DELETE"] = 2;
	(*alg)["options"] = (*alg)["OPTIONS"] = 2;
	(*alg)["trace"] = (*alg)["TRACE"] = 2;
	(*alg)["connect"] = (*alg)["CONNECT"] = 2;

	///<connect
	(*alg)["close"] = 0;
	(*alg)["keep-alive"] = 1;
}
/**
*@brief Httpworker
*@author  ACM2012
*@return void
*@note
* Httpworker的析构函数实现
*/
Httpworker::~Httpworker()
{
	delete msg;
	delete rmsg;
	delete alg;
	host = "";
	data.clear();
}
/**
* @author ACM2012
* @brief 分隔Message成多个部分
* @param [in] <Msg> string型
* @param [in] <split> char型
* @return int型数据
* @note
*/
int Httpworker::div(string Msg, char split){
	data.clear();
	if (Msg.size() == 0){
		return -1;///< none msg error
	}
	///<div
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
		return -1;///< lost argc
	data[0] = STR::Upper(data[0]);
	i = data[1].find('/');
	this->host = data[1].substr(0, i);
	//data[1] = data[1].substr(i + 1);
	if (i == -1 || (this->host+'/'==data[1])){
		data[1] = "/";
	}
	else data[1] = data[1].substr(i);
	return 1;///<return success 
}
/**
* @author ACM2012
* @brief 定义报文字段的属性
* @return 无
* @note
*/
void Httpworker::Make(){
	if(msg) delete msg; ///<如果存在msg，删除它
	msg = new HttpMsg();///<更新msg
	msg->method = data[0];///<msg的方法为data[0]，即命令
	msg->path = data[1];///< msg的路径为data[1]
	msg->no = "HTTP/1.1";///<版本号为HTTP/1.1
	msg->host = this->host;///<主机为本地主机
	msg->user = "HUST ACM2012 Virtual Terminal - v1.0";///<msg的用户为HUST ACM2012 Virtual Terminal - v1.0
	msg->language = "zh-CN";///<默认设置语言为zh-CN
	msg->connect = "close";///<默认连接为close
	msg->accept = "text/html";///<默认接收文件为text/html格式
	msg->accept_enconding = "gzip,deflate,sdch";///<默认编码方式为gzip,deflate,sdch
	string path;
	int len = msg->path.length();///<msg的长度
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
/**
* @author ACM2012
* @brief http协议命令相应码的分析
* @return int型数据
* @note
*/
int Httpworker::analy(){
	if (rmsg->no != msg->no){
		return -1;///<no version
	}
	///<not much analy 
	///<web cache is right
	string path;
	int len = msg->path.length();
	if (msg->path[len-1]=='/') 
		path = "cache/" + msg->path+"index.html";
	else path = "cache/" + msg->path;
	if (rmsg->code == "304"){
		rmsg->data = FIO::ReadFullFile(path);
	}
	else if (rmsg->code == "200" && (msg->method=="get"||msg->method=="GET")){
		///<new cache page
		if (FIO::Exist(path)){
			FIO::DelFile(path);
			FIO::DelFile(path + ".cache");
		}
		FIO::SaveFile(path, &rmsg->data);
		FIO::SaveFile(path + ".cache", &rmsg->last_modified);
	}
	///<rmsg->cookie.create_host(ip);
	return 1;///<success
}
/**
* @author ACM2012
* @brief 监听应答报文
* @return string型数据
* @note
*/
string Httpworker::look_rmsg(){
	string look;
	string a, b, c;
	c = STR::int2string(rmsg->length);
	look = a + "\n" + b + "\n" + rmsg->word + " " + rmsg->connect + "\n";
	look += rmsg->date + "\n" + rmsg->server + "\n" + rmsg->last_modified + "\n" + c + "\n";
	look += rmsg->type[0] + "\n" + "cookie\n" + rmsg->data;
	return look;
}

/**
* @brief 监听请求报文
* @author ACM2012
* @return CString
* @note
* return all string in msg
*/
string Httpworker::look_msg(){
	string Look;
	string a;
	Look = msg->method + " " + msg->path + " " + msg->no + "\n";
	Look += "host: "+msg->host + "\n" +" User-Agent: " +msg->user + "\n";
	Look += msg->language + "\n" + msg->connect + "\n";
	Look += msg->if_modified_since + "\n";
	if (msg->cookie.num){
		Look += STR::int2string(msg->cookie.num) + '\n';
	}
	else Look += "cookie\n";
	Look += msg->data;
	return Look;
}
/**
* @brief 显示应答报文
* @author ACM2012
* @return CString
* @note
* 应答报文应该包括Version、Error_code、Date、Last_modified、Type、Data
*/
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
/**
* @brief 设置请求报文
* @author ACM2012
* @return int型数据
* @note
* 应答报文应该包括Version、Error_code、Date、Last_modified、Type、Data
*/
int Httpworker::setMsg(string rec){
	vector<string> d;
	STR::Split(rec, &d, "\r\n");
	rmsg = new HttpRMsg();
	int i;
	///<the begining must be exist
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
/**
* @brief 获取请求报文
* @author ACM2012
* @return string型数据
* @note
* 请求报文的结构字段详细描述
*/
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
	///<data
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
