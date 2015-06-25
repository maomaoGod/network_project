/**
*@file
*@brief HttpЭ��ͻ��˵�ʵ��
*@author ACM2012
*@date 2015/06/06
*@version 1.1
*@note
* ʵ��httpЭ��ͻ��˵Ĺ��ܣ�ʵ��cookie��
* ��������ĺ�Ӧ���ĵĽṹ��
*/
#pragma once
#include <string>
#include <vector>
#include <map>
#include <time.h>
#include "Tools.h"

using namespace Tools;
using namespace std;

typedef string version; /**@brief ����versionΪstring*/
typedef string http_code;/**@brief ����http_codeΪstring*/
typedef map<string, int> checkarg; /**@brief ����checkargΪmap<string, int>*/
typedef string IP;/**@brief ����IPΪstring*/
#define MarkHttp "\r\n"/**@brief ����MarkHttpΪ"\r\n"*/
/**
*@brief Http cookie
*@author ACM2012
*@note
* ���û����η���ʱ������cookie��Ϣ����cookie�汾�ź���ʷ��¼�ȡ�
* ���û��������ʹ������ҳ����Ӧ��cookie��Ϣ������¼�����
*/
struct Httpcookie{
	int num;/**@brief cookie �汾��*/
	string history;/**@brief ��ʷ��¼*/

	/**
	* @author ACM2012
	* @return ����cookie�汾��
	* @note
	* ����������ķ���������cookie�汾��
	* �汾�ŷ�ΧΪ1000~9999
	* @remarks
	*/
	int RandCook(){
		num = 1000 + rand() % 9000;/**@brief  �汾�ŷ�ΧΪ1000~9999*/
		return num;/**@brief  ����cookie�汾��*/
	}

	/**
	* @author ACM2012
	* @return ����true or false
	* @note
	* �ҳ�cookie��·��
	* ��������ҵ�����true��
	* ���򷵻�false
	* @remarks
	*/
	bool findcookie(){
		string path = "cache/cookie/" + STR::int2string(num) + ".cookie";/**@brief  cookie·����ʽΪcache/cookie/" + STR::int2string(num) + ".cookie*/
		return FIO::Exist(path);/**@brief  ���غ������*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int�ͣ���������cookie
	* @return ����true or false
	* @note
	* ���û���ҳ�cookie��·����˵�����û�Ϊ���η���
	* ����Ҫ����cookie
	* ��������ɹ�����true��
	* ���򷵻�false
	* @remarks
	*/
	bool create(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";/**@brief  cookie·����ʽΪcache/cookie/" + STR::int2string(num) + ".cookie*/
		if (FIO::Exist(path)){ /**@brief  ������ڸ�·������ɾ��*/
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;/**@brief  �������cookie��Ϣ��ʽΪSTR::int2string(num) + "|" + history*/
		FIO::SaveFile(path, &w);/**@brief  ����cookie��Ϣ��ָ���ļ�*/
		return true;/**@brief  ���غ������*/
	}

	/**
	* @author ACM2012
	* @param [in] <host> string�ͣ���������host
	* @return ����true or false
	* @note
	* ���û���ҳ�host��·����˵�����û�Ϊ���η���
	* ����Ҫ����host
	* ��������ɹ�����true��
	* ���򷵻�false
	* @remarks
	*/
	bool create_host(string host){
		string path = "cache/cookie/" + host + ".cookie";/**@brief  host·����ʽΪcache/cookie/" + STR::int2string(num) + ".cookie*/
		if (FIO::Exist(path)){ /**@brief  ������ڸ�·������ɾ��*/
			FIO::DelFile(path);
		}
		string w = STR::int2string(num) + "|" + history;/**@brief  �������host��Ϣ��ʽΪSTR::int2string(num) + "|" + history*/
		FIO::SaveFile(path, &w);/**@brief  ����host��Ϣ��ָ���ļ�*/
		return true;/**@brief  ���غ������*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int��
	* @return ����true or false
	* @note
	* ��ȡcookie�����ҽ�cookie·���е���Ϣ��ȡ����
	* �����ȡ�ɹ�
	* ����'\n'���ָ�·����Ϣ����·������ɲ��ִ���ָ������
	* �Ӷ��õ�cookie�汾�ź���ʷ��¼������true
	* ���򷵻�false
	* @remarks
	*/
	bool readcook(int cookie){
		string path = "cache/cookie/" + STR::int2string(cookie) + ".cookie";/**@brief  cookie·����ʽΪcache/cookie/" + STR::int2string(num) + ".cookie*/
		if (!FIO::Exist(path)){ /**@brief  ��������ڸ�·�����򷵻�false*/
			return false;
		}
		string r = FIO::ReadFile(path, 0);/**@brief  ���ļ��ж�ȡpath����λ�õ���Ϣ���������r*/
		vector<string> d;/**@brief  ����һ��vector<string>�͵ı��������ڷ����ȡ·����Ϣ�ĸ�����ɲ���*/
		STR::Split(r, &d, '\n');/**@brief  ��'\n'���ָ�·������·������ɲ��ִ���d*/
		num = STR::string2int(d[0]);/**@brief  ��cookie�汾�ţ���d[0],����num*/
		history = d[1];/**@brief  ��cookie��ʷ��¼����d[1],����history*/
		return true;/**@brief  ���غ������*/
	}

	/**
	* @author ACM2012
	* @param [in] <cookie> int��,��ʼֵΪ0
	* @param [in] <host> string��,��ʼֵΪ"",��Ϊ���ַ���
	* @return ����true or false
	* @note
	* ��ȡcookie_host�����ҽ�cookie_host·���е���Ϣ��ȡ����
	* �����ȡ�ɹ�
	* ����'|'���ָ�·����Ϣ����·������ɲ��ִ���ָ������
	* �Ӷ��õ�cookie_host�汾�ź���ʷ��¼������true
	* ���򷵻�false
	* @remarks
	*/
	bool readcook_host(string host = "", int cookie = 0){
		string path = "cache/cookie/" + host + ".cookie";/**@brief  cookie_host·����ʽΪcache/cookie/" +  host + ".cookie*/
		if (!FIO::Exist(path)){/**@brief  ��������ڸ�·�����򷵻�false*/
			return false;
		}
		string r = FIO::ReadFile(path, 0);/**@brief  ���ļ��ж�ȡpath����λ�õ���Ϣ���������r*/
		vector<string> d;/**@brief  ����һ��vector<string>�͵ı��������ڷ����ȡ·����Ϣ�ĸ�����ɲ���*/
		STR::Split(r, &d, '|');/**@brief  ��'|'���ָ�·������·������ɲ��ִ���d*/
		num = STR::string2int(d[0]);/**@brief  ��cookie_host�汾�ţ���d[0],����num*/
		history = d[1];;/**@brief  ��cookie_host��ʷ��¼����d[1],����history*/
		return true;/**@brief  ���غ������*/
	}
};
/**
*@brief HttpЭ��ͻ��˱��Ľṹ
*@author ACM2012
*@note
* ����HttpЭ��ͻ��˵�Message
*/
struct HttpMsg{
	///<request
	struct{
		string method;/**@brief  ��ʽ*/
		string path;/**@brief  ·��*/
		version no;/**@brief  �汾��*/
	};
	///<host
	struct{
		string host;/**@brief  host*/
		string user;/**@brief  �û�*/
		int length;/**@brief  ����*/
		string language;/**@brief  ����*/
		string connect;/**@brief  ����*/
		string if_modified_since;/**@brief  web cache*/
		string accept;
		string accept_enconding;
	};
	Httpcookie cookie;
	string data;
};
/**
*@brief HttpЭ��ͻ���Request Message
*@author ACM2012
*@note
* ����HttpЭ��ͻ��˵�Request Message
*/
struct HttpRMsg{
	struct{
		version no;/**@brief  �汾��*/
		http_code code;
		string word;/**@brief  �汾��*/
	};
	struct{
		string connect;/**@brief  ����*/
		string date;/**@brief  ����*/
		string server;/**@brief  ������*/
		string last_modified;/**@brief  ����޸İ汾*/
		int length;/**@brief  ��Ϣ�ĳ��ȴ�С*/
		vector<string> type;/**@brief  ����*/
		string vary;
		string accapt_ranges;
	};
	Httpcookie cookie;/**@brief  cookie�汾��*/
	string data;/**@brief  ��������*/

	/**
	* @brief
	* @param [in] <msg> string��
	* @author ACM2012
	* @return ��
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
	* @param [in] <msg> string��
	* @author ACM2012
	* @return ��
	* @note
	*  �ҳ�msg���ײ�
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
	Httpworker();/**@brief  Httpworker���캯�� */
	~Httpworker();/**@brief  Httpworker�������� */
	int div(string Msg, char split);/**@brief  �ָ�Message�ɶ������ */
	void Make();
	int analy();/**@brief  httpЭ��������Ӧ��ķ��� */
	/**
	* @brief
	* @param [in] <rec> string��
	* @author ACM2012
	* @return ���óɹ�������true�����򣬷���false
	* @note
	*  Set Message
	*/
	int setMsg(string rec);
	/**
	* @brief
	* @author ACM2012
	* @return ��ȡ������Ϣ
	* @note
	*  Get Message
	*/
	string getMsg();
	/**
	* @brief
	* @author ACM2012
	* @return ��ȡ����host
	* @note
	*  Get Host
	*/
	string gethost(){
		return host;
	}
	/**
	* @brief
	* @author ACM2012
	* @return ��ȡ����IP
	* @note
	*  Get IP
	*/
	IP getIP(){
		return ip;
	}
	/**
	* @brief
	* @param [in] <ip> IP��
	* @author ACM2012
	* @return ��
	* @note
	*  Set IP
	*/
	void setIP(IP ip){
		this->ip = ip;
	}
	/**
	* @brief
	* @author ACM2012
	* @return ������󣬷���true�����򣬷���false
	* @note
	*  Check IP:
	*  ���IP��ַ���з����ֶ��Ҳ���'.'ʱ������false
	*  ���򣬷���true
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
	* @return �˿ں�
	* @note
	*  ��ȡ�˿ں�
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

	/**@brief ˽�б���*/
private:
	HttpMsg *msg;/**@brief  Http Message*/
	HttpRMsg *rmsg;/**@brief  Http Request Message*/
	checkarg *alg;/**@brief ���ӳ���ϵʱ��������*/
	string host; /**@brief  host����*/
	IP ip;	/**@brief IP Address*/
	vector<string> data;	/**@brief ��������*/
	int Port;/**@brief �˿ں�*/
};

