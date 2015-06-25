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
#pragma once
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "Tools.h"

using namespace Tools;
using namespace std;

typedef string version; /**@brief 定义version为string*/
typedef string http_code;/**@brief 定义http_code为string*/
typedef map<string, int> checkarg; /**@brief 定义checkarg为map<string, int>*/
typedef string IP;/**@brief 定义IP为string*/
#define MarkHttp "\r\n"/**@brief 定义MarkHttp为"\r\n"*/
/**
*@brief Http cookie
*@author ACM2012
*@note
* 当用户初次访问时，创建cookie信息，即cookie版本号和历史记录等、
* 若用户曾经访问过，则找出其对应的cookie信息，并记录其操作
*/
struct Httpcookie{
	int num;/**@brief cookie 版本号*/
	string history;/**@brief 历史记录*/

	/**
	* @author ACM2012
	* @return 返回cookie版本号
	* @note
	* 采用随机数的方法，生成cookie版本号
	* 版本号范围为1000~9999
	* @remarks
	*/
	int RandCook(){
		num = 1000 + rand() % 9000;/**@brief  版本号范围为1000~9999*/
		return num;/**@brief  返回cookie版本号*/
	}

	/**
	* @author ACM2012
	* @return 返回true or false
	* @note
	* 找出cookie的路径
	* 如果可以找到返回true；
	* 否则返回false
	* @remarks
	*/
	bool findcookie(){
		string path = "cache/cookie/" + STR::int2string(num) + ".cookie";/**@brief  cookie路径格式为cache/cookie/" + STR::int2string(num) + ".cookie*/
		return FIO::Exist(path);/**@brief  返回函数结果*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int型，待创建的cookie
	* @return 返回true or false
	* @note
	* 如果没有找出cookie的路径，说明该用户为初次访问
	* 则需要创建cookie
	* 如果创建成功返回true；
	* 否则返回false
	* @remarks
	*/
	bool create(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";/**@brief  cookie路径格式为cache/cookie/" + STR::int2string(num) + ".cookie*/
		if (FIO::Exist(path)){ /**@brief  如果存在该路径，则删除*/
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;/**@brief  待保存的cookie信息格式为STR::int2string(num) + "|" + history*/
		FIO::SaveFile(path, &w);/**@brief  保存cookie信息到指定文件*/
		return true;/**@brief  返回函数结果*/
	}

	/**
	* @author ACM2012
	* @param [in] <host> string型，待创建的host
	* @return 返回true or false
	* @note
	* 如果没有找出host的路径，说明该用户为初次访问
	* 则需要创建host
	* 如果创建成功返回true；
	* 否则返回false
	* @remarks
	*/
	bool create_host(string host){
		string path = "cache/cookie/" + host + ".cookie";/**@brief  host路径格式为cache/cookie/" + STR::int2string(num) + ".cookie*/
		if (FIO::Exist(path)){ /**@brief  如果存在该路径，则删除*/
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;/**@brief  待保存的host信息格式为STR::int2string(num) + "|" + history*/
		FIO::SaveFile(path, &w);/**@brief  保存host信息到指定文件*/
		return true;/**@brief  返回函数结果*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int型
	* @return 返回true or false
	* @note
	* 读取cookie，并且将cookie路径中的信息提取出来
	* 如果读取成功
	* 则以'\n'来分隔路径信息，将路径的组成部分存入指定变量
	* 从而得到cookie版本号和历史记录，返回true
	* 否则返回false
	* @remarks
	*/
	bool readcook(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";/**@brief  cookie路径格式为cache/cookie/" + STR::int2string(num) + ".cookie*/
		if (!FIO::Exist(path)){ /**@brief  如果不存在该路径，则返回false*/
			return false;
		}
		string r = FIO::ReadFile(path, 0);/**@brief  从文件中读取path所在位置的信息，存入变量r*/
		vector<string> d;/**@brief  声明一个vector<string>型的变量，用于方便存取路径信息的各个组成部分*/
		STR::Split(r, &d, '\n');/**@brief  以'\n'来分隔路径，将路径的组成部分存入d*/
		num = STR::string2int(d[0]);/**@brief  将cookie版本号，即d[0],赋给num*/
		history = d[1];/**@brief  将cookie历史记录，即d[1],赋给history*/
		return true;/**@brief  返回函数结果*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int型,初始值为0
	* @param [in] <host> string型,初始值为"",即为空字符串
	* @return 返回true or false
	* @note
	* 读取cookie_host，并且将cookie_host路径中的信息提取出来
	* 如果读取成功
	* 则以'|'来分隔路径信息，将路径的组成部分存入指定变量
	* 从而得到cookie_host版本号和历史记录，返回true
	* 否则返回false
	* @remarks
	*/
	bool readcook_host(string host = "", int cookie = 0){
		string path = "cache/cookie/" + host + ".cookie";/**@brief  cookie_host路径格式为cache/cookie/" +  host + ".cookie*/
		if (!FIO::Exist(path)){/**@brief  如果不存在该路径，则返回false*/
			return false;
		}
		string r = FIO::ReadFile(path, 0);/**@brief  从文件中读取path所在位置的信息，存入变量r*/
		vector<string> d;/**@brief  声明一个vector<string>型的变量，用于方便存取路径信息的各个组成部分*/
		STR::Split(r, &d, '|');/**@brief  以'|'来分隔路径，将路径的组成部分存入d*/
		num = STR::string2int(d[0]);/**@brief  将cookie_host版本号，即d[0],赋给num*/
		history = d[1];;/**@brief  将cookie_host历史记录，即d[1],赋给history*/
		return true;/**@brief  返回函数结果*/
	}
};
/**
*@brief Http协议客户端报文结构
*@author ACM2012
*@note
* 定义Http协议客户端的Message
*/
struct HttpMsg{
	///<request
	struct{
		string method;/**@brief  方式*/
		string path;/**@brief  路径*/
		version no;/**@brief  版本号*/
	};
	///<host
	struct{
		string host;/**@brief  host*/
		string user;/**@brief  用户*/
		int length;/**@brief  长度*/
		string language;/**@brief  语言*/
		string connect;/**@brief  连接*/
		string if_modified_since;/**@brief  web cache*/
		string accept;
		string accept_enconding;
	};
	Httpcookie cookie;
	string data;
};
/**
*@brief Http协议客户端Request Message
*@author ACM2012
*@note
* 定义Http协议客户端的Request Message
*/
struct HttpRMsg{
	struct{
		version no;/**@brief  版本号*/
		http_code code;
		string word;/**@brief  版本号*/
	};
	struct{
		string connect;/**@brief  类型*/
		string date;/**@brief  日期*/
		string server;/**@brief  服务器*/
		string last_modified;/**@brief  最后修改版本*/
		int length;/**@brief  信息的长度大小*/
		vector<string> type;/**@brief  类型*/
		string vary;
		string accapt_ranges;
	};
	Httpcookie cookie;/**@brief  cookie版本号*/
	string data;/**@brief  数据内容*/

	/**
	* @brief
	* @param [in] <msg> string型
	* @author ACM2012
	* @return 无
	* @note
	*  Set Code
	*/
	void setCode(string msg){
		vector<string> d;
		STR::Split(msg, &d, ' ');
		no = d[0];
		code = d[1];
		word = d[2];
	}
	/**
	* @brief
	* @param [in] <msg> string型
	* @author ACM2012
	* @return 无
	* @note
	*  找出msg的首部
	*/
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


/**
*@class Httpworker Httpworker.h "HustNet/Httpworker.h"
*@brief How the Http protocol works
*@author  ACM2012
*@note
*to achieve communication of the client and the server with HTTP protocol
*/
class Httpworker
{
public:
	Httpworker();/**@brief  Httpworker构造函数 */
	~Httpworker();/**@brief  Httpworker析构函数 */
	int div(string Msg, char split);/**@brief  分隔Message成多个部分 */
	void Make();
	int analy();/**@brief  http协议命令相应码的分析 */
	/**
	* @brief
	* @param [in] <rec> string型
	* @author ACM2012
	* @return 设置成功，返回true；否则，返回false
	* @note
	*  Set Message
	*/
	int setMsg(string rec);
	/**
	* @brief
	* @author ACM2012
	* @return 获取到的信息
	* @note
	*  Get Message
	*/
	string getMsg();
	/**
	* @brief
	* @author ACM2012
	* @return 获取到的host
	* @note
	*  Get Host
	*/
	string gethost(){
		return host;
	}
	/**
	* @brief
	* @author ACM2012
	* @return 获取到的IP
	* @note
	*  Get IP
	*/
	IP getIP(){
		return ip;
	}
	/**
	* @brief
	* @param [in] <ip> IP型
	* @author ACM2012
	* @return 无
	* @note
	*  Set IP
	*/
	void setIP(IP ip){
		this->ip = ip;
	}
	/**
	* @brief
	* @author ACM2012
	* @return 检查无误，返回true；否则，返回false
	* @note
	*  Check IP:
	*  如果IP地址中有非数字而且不是'.'时，返回false
	*  否则，返回true
	*/
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
	/**
	* @brief
	* @author ACM2012
	* @return 端口号
	* @note
	*  获取端口号
	*/
	int getPort(){
		if (Port < 1024){
			Port = 80;
		}
		return Port;
	}

	/**@brief  return all string in msg */
	string look_msg();
	/**@brief  return all string in rmsg */
	string look_rmsg();
	/**@brief   show all string in rmsg */
	CString show_rmsg();

	/**@brief 私有变量*/
private:
	HttpMsg *msg;/**@brief  Http Message*/
	HttpRMsg *rmsg;/**@brief  Http Request Message*/
	checkarg *alg;/**@brief 添加映射关系时，检查参数*/
	string host; /**@brief  host声明*/
	IP ip;	/**@brief IP Address*/
	vector<string> data;	/**@brief 数据内容*/
	int Port;/**@brief 端口号*/
};

