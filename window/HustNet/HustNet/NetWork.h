/**@file
 *@brief sending and receiving message from server
 *@author ACM2012
 *@date 2015/04/19
 *@version 0.1
 *@note
 *As to send message, the format is CMD + " " + PATH + " " + HOST
 *As to receive message, the format is FILE TYPE + " " + FILE LEN + " " + FILE CONTENT
 */

#pragma once
#include "stdafx.h"
#include "map"
#include "Tools.h"
#include "UICtrl.h"
#include "DNSworker.h"
//#include "Httpworker.h"
#include "Ftpworker.h"
#include "FtpDataSocket.h"
//#import "dll/JHttp.dll"

#define HTTP_PORT 6500

using namespace std;
using namespace Tools;

namespace NetWork{
	/**
	 *@class ChatWork NetWork
	 *@brief Socket
	 *@author   ACM2012
	 *@note
	 *XXXXXXXXX
	 */
	class ChatWork{
	public:
		ChatWork(){
			init();
			Obj = _T("Public");
			/**@brief local*/
			No = -1;
			/**@brief server*/
			TNo = _T("-1");
			this->Name = "No1";
		}
		ChatWork(CString name){
			ChatWork();
			this->Name = name;
		}
		~ChatWork(){
		}
		void Begin(){
			CString mystr;
			CString Code = _T("Chat ") + this->Name + _T(">");
			TakeOverCmd(Code);
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CStringArray cmd;
				STR::CCarg(&cmd, mystr, _T(' '));
				PrintLog(_T("Accept ") + cmd[0]);
				switch (exp[cmd[0]]){
                /**@brief keep*/
				case 0: break;
                /**@brief send*/
				case 1:
					if (cmd.GetSize() < 2){
						PrintLog(_T("Error code for no Msg"));
						break;
					}
					SendMsg(cmd[1]);
					break;
                /**@brief link*/
				case 2: break;
				case 3:
					if (cmd.GetSize() < 2){
						PrintLog(_T("Error code for no Msg"));
						break;
					}
					ConnMsg(cmd[1]);
					break;
				case 10:
					CNo.Format(_T("%d"), this->No);
					PrintRp(_T("the no of client is ") + CNo);
					PrintRp(_T("the aim of client is ") + TNo);
					break;
				default: PrintLog(_T("Error Code"));
				}
			}
			if (!Finish()) { PrintLog(_T("Delete CSocket failed\n")); }
		}
		bool Finish(){
			csocket.Close();
			return true;
		}
	private:
		map<CString, int> exp;
		CString Name;
		int No;	
		CString Obj;///< CString Msg*/
		CString CNo, TNo;
		CSocket csocket;
		CString strIP;///< IP*/
		TCHAR szRecValue[1024];
		void init(){
		    /**@brief Send message to someone*/
			exp[_T("Send")] = exp[_T("send")] = 1;
			/**@brief link to somebody*/
			exp[_T("Link")] = exp[_T("link")] = 2;
			/**@brief connect*/
			exp[_T("Conn")] = exp[_T("conn")] = 3;
			exp[_T("Info")] = 10;
		}
		void SendMsg(CString Msg){
			CString rev;
			memset(szRecValue, 0, sizeof(szRecValue));
			/**brief send message to the server*/
			PrintLog(_T("���ӷ������ɹ�"));
			csocket.Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			csocket.Receive((void *)szRecValue, 1024);
			rev.Format(_T("%s"), szRecValue);
			PrintRp(rev);
		}
		void ConnMsg(CString Msg){
			AfxSocketInit();
			strIP.Format(_T("%s"), _T("127.0.0.1"));
			if (!csocket.Create())
			{
				CString error;
				error.Format(_T("����ʧ��:%d"), csocket.GetLastError());
				PrintLog(error);
				return;
			}
			/**@brief ת����Ҫ���ӵĶ˿���������*/
			PrintLog(_T("�׽��ִ����ɹ�"));
			/**@brief ����ָ���ĵ�ַ�Ͷ˿�*/
			if (csocket.Connect(strIP, 6500))
			{
				CString rev;
				memset(szRecValue, 0, sizeof(szRecValue));
				/**brief send message to the server*/
				PrintLog(_T("���ӷ������ɹ�"));
				csocket.Send(Msg, Msg.GetLength()*sizeof(TCHAR));
				csocket.Receive((void *)szRecValue, 1024);
				rev.Format(_T("%s"), szRecValue);
			}
			else{
				CString error;
				error.Format(_T("����ʧ��:%d"), csocket.GetLastError());
				PrintLog(error);
			}
		}

	};


	//take over the system
	class FTPApp{
	public:
		FTPApp(){
			AfxSocketInit();
			ftpworker = new Ftpworker();
		}
		~FTPApp(){
			delete ftpworker;
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myftp>"));
			aSocket = new CSocket();
			//create a Socket
			if (!aSocket->Create()){
				CString error;
				error.Format(_T("����ʧ��:%d"), aSocket->GetLastError());
				PrintLog(error);
				return;
			}
			//take over the cmd
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				if (ftpworker->div(STR::CS2S(mystr), ' ') < 0){
					PrintRp(_T("argc miss"));
					continue;
				}
				//first conn
				int error_code = ftpworker->dealwith();
				if (error_code < 0){
					PrintRp(_T("NOP code or not set the SendWay"));
					continue;
				}
				if (!error_code){//dealwith
					if (aSocket->Connect(STR::S2CS(ftpworker->getIP()), ftp_port)){
						aSocket->Receive((void *)szRecValue, 1024);
						rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
						PrintRp(rev);
						ftpworker->connected = true;
						continue;
					}
					else{
						CString error;
						error.Format(_T("���ӷ�����ʧ��:%d"), aSocket->GetLastError());
						PrintLog(error);
						return;
					}
				}
				//not use conn cmd
				if (!ftpworker->connected){
					PrintRp(_T("not connect!"));
					ftpworker->connected = false;
					continue;
				}
				//send data and rev
				
				if (error_code == 2){
					if (ftpworker->getWay() < 0){
						PrintRp(_T("not set you send style"));
						continue;
					}
				}/*
				if (error_code == 3){
					//
					if (ftpworker->getWay() == 1){
						//set a port to listen
						dataworker = new FtpDataSocket();
						dataworker->Create(ftpworker->GetPort());
						dataworker->Listen();
					}
					else{
						//wait for us to send a data auto;
					}
				}*/
				if (error_code){
					if (!Send()){
						PrintLog(_T("Interrupt Error!"));
						return;
					}
					Rev();
					/*if (error_code == 2){
						//use different way to tarns message
						if (ftpworker->getWay() == 1){
							//set a port to listen
							dataworker->src_path = ftpworker->getSrc();
							dataworker->des_path = ftpworker->getDes();
							dataworker->style = ftpworker->getStyle();
							dataworker->good = true;
							//begin to send
						}
						else{
							//wait for us to send a data auto;
							bSocket = new CSocket();
							bSocket->Create();
							bSocket->Connect(STR::S2CS(ftpworker->getIP()), data_port);
							TCHAR buf[1024];
							memset(buf, 0, sizeof(buf));
							bSocket->Receive((void *)buf, 1024);
							memset(buf, 0, sizeof(buf));
							CString sendmsg=_T("null");
							if (ftpworker->getStyle()){//download
								bSocket->Send(sendmsg, sizeof(TCHAR)*sendmsg.GetLength());
								CString rev;
								CString temp;
								while (bSocket->Receive((void *)buf, 1024)){
									temp.Format(_T("%s"), buf);
									rev += temp;
									memset(buf, 0, 1024 * sizeof(TCHAR));
								}
								FIO::SaveFile(ftpworker->getDes(), &STR::CS2S(rev));
							}
							else{//upload
								sendmsg = STR::S2CS(FIO::ReadFile(ftpworker->getSrc(),1));
								bSocket->Send(sendmsg, sizeof(TCHAR)*sendmsg.GetLength());
								bSocket->Receive((void *)buf, 1024);
							}
							bSocket->Close();
							delete bSocket;
						}
					}*/
					ftpworker->Dealfile(error_code);
				}
				// according to different code to diff thing
				/*
				if (error_code){
					//send data and rec data
					//it may be open last time
					bSocket = new CSocket();
					bSocket->Close();
					delete bSocket;
				}*/
			}
			aSocket->Close();
			delete aSocket;
		}
	
	private:
		FtpDataSocket *dataworker;
		Ftpworker *ftpworker;
		CSocket *aSocket;
		CSocket *bSocket;
		CString IP;
		CString Path;
		CString rev;
		TCHAR szRecValue[1024];
		string save;

		bool Send(){
			CString Msg;
			Msg = STR::S2CS(ftpworker->getMsg());
			save = ftpworker->getSmallData();
			//aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			TCHAR *senddata;
			int len = STR::CString2TCHAR(&senddata, &Msg);
			//aSocket->Send(data, data.GetLength()*sizeof(TCHAR));
			int i;
			for (i = 0; i < len; i += BUFSIZE){
				if(i+BUFSIZE < len)
					aSocket->Send(senddata + i, BUFSIZE*sizeof(TCHAR));
				else{
					CString str = senddata + i;
					str += STR::FillStr(_T('\n'), BUFSIZE - (len % BUFSIZE));
					aSocket->Send(str, BUFSIZE*sizeof(TCHAR));
				}
			}
			//Msg = _T("ftp ask end");
			//aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		
		void Rev(){
			//need analy
			rev = _T("");
			CString temp;
			int count;
			while (true){
				count = 0;
				memset(szRecValue, 0, BUFSIZE * sizeof(TCHAR));
				//aSocket->Receive((void *)szRecValue, BUFSIZE);
				while (true){
					count += aSocket->Receive((void *)(szRecValue+count), (BUFSIZE-count));
					if (count == BUFSIZE) break;
				}
				temp.Format(_T("%s"), szRecValue);
				string s = STR::RemoveStr(STR::CS2S(temp), '\n');
				if (s[0] == '\0')
					break;
				rev += temp;
			}
			/*
			CString temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			aSocket->Receive((void *)szRecValue, 1024);
			temp.Format(_T("%s"), szRecValue);*/
			ftpworker->setMsg(STR::CS2S(rev));
			if (ftpworker->Analy() > 0){
				//show it and do something else
				PrintRp(ftpworker->show_msg(save));
			}
		}

	};

	/**
	 *@class AppLayerHttp NetWork
	 *@brief How the Application Layer works
	 *@author   ACM2012
	 *@note
	 *to achieve communication of the client and the server with HTTP protocol
	 */

typedef void *(*HttPtr)(void);
typedef int(*Httpdiv)(void *, string, char);
typedef void(*HttpSetport)(void *, int);
typedef int(*HttpgetPort)(void*);
typedef void (*HttpMake)(void *);
typedef int(*Httpanaly)(void*);
typedef int(*HttpSetMsg)(void *, string);
typedef string(*HttpgetMsg)(void*);
typedef string(*Httpgethost)(void*);
typedef void(*Httpsetdata)(void*, string);
typedef string(*Httpgetdata)(void*);
typedef IP(*HttpgetIP)(void*);
typedef void(*HttpsetIp)(void*,IP);
typedef bool(*HttpIPcheck)(void*);
typedef string(*Httplook_msg)(void*);
typedef string(*Httplook_rmsg)(void*);
typedef CString(*Httpshow_rmsg)(void*);

	class AppLayerHttp{
	public:
		AppLayerHttp(){
			AfxSocketInit();
			dll = new NetDll();
			dll->OpenDll();
			//httpworker = new Httpworker();
			httptr = (HttPtr)dll->GetFun("NewHttptr"); 
			httpsetport = (HttpSetport)dll->GetFun("setPort"); 
			httpdiv = (Httpdiv)dll->GetFun("Httpdiv");
			httpmake = (HttpMake)dll->GetFun("Make");
			httpgetPort = (HttpgetPort)dll->GetFun("getPort");
			httpgethost = (Httpgethost)dll->GetFun("gethost");
			httpgetMsg = (HttpgetMsg)dll->GetFun("getMsg");
			httpsetmsg = (HttpSetMsg)dll->GetFun("setMsg");
			httpgetdata = (Httpgetdata)dll->GetFun("getdata");
			httpsetdata = (Httpsetdata)dll->GetFun("setdata");
			httpanaly = (Httpanaly)dll->GetFun("analy");

			httpworker = httptr();
			dnsworker = new DNSworker();
			//httpworker->setPort(80);
			httpsetport(httpworker, 80);
		}
		~AppLayerHttp(){
			dll->FreeDll();
			delete httpworker;
			delete dnsworker;
		}
		
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myhttp>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				CStringArray code;
				//httpworker->div(STR::CS2S(mystr), ' ')
				int divtmp = httpdiv(httpworker, STR::CS2S(mystr), ' ');
				//int divtmp = httpworker->div(STR::CS2S(mystr), ' ');
				if (divtmp < 0){
					vector<string> d;
					STR::Split(STR::CS2S(mystr), &d, ' ');
					if (d.size()>1){
						if (STR::Upper(d[0]) == "SETPORT"){
							//httpworker->setPort(STR::string2int(d[1]));
							httpsetport(httpworker, STR::string2int(d[1]));
							PrintLog(_T("Port ")+STR::S2CS(d[1])+_T(" successfully"));
							continue;
						}
					}
					PrintLog(_T("argc error"));
					continue;
				}
				aSocket = new CSocket();
			    if (!aSocket->Create()){
					CString error;
				    error.Format(_T("����ʧ��:%d"), aSocket->GetLastError());
				    PrintLog(error);
				    return;
			    }
				if (!Send()){
					PrintLog(_T("Interrupt Error!"));
					return;
				}
				Rev();
				aSocket->Close();
				delete aSocket;
			}
		}
		/**
         *@brief the client sends a report to the server
         *@author  ACM2012
         *@param [in] <Method> HTTP CMD
         *@param [in] <url> the link which waiting to access
         *@return True if successfully, otherwise False
         *@note
         *1 div url to path + IP
		 *2 com Method + " " + path + IP
	     *3 send com Msg
		 *   return true if TCP finish
		 *   else return false
         *@remarks the format of sending message is "CMD + " " + PATH + " " + HOST"
		 */	
		bool Send(){
			//httpworker->Make();
			httpmake(httpworker);
			//if (httpworker->getPort() == 80){
			if (httpgetPort(httpworker) == 80){
				//if (aSocket->Connect(STR::S2CS(httpworker->gethost()), httpworker->getPort())){
				if (aSocket->Connect(STR::S2CS(httpgethost(httpworker)), httpgetPort(httpworker))){
					PrintRp(_T("link to ") + STR::S2CS(httpgethost(httpworker)));//rev
				}
				else{
					CString error;
					error.Format(_T("���ӷ�����ʧ��:%d"), aSocket->GetLastError());
					PrintLog(error);
					return false;
				}
			}
			else{
				//if (aSocket->Connect(STR::S2CS(httpworker->gethost()), httpworker->getPort())){
				if (aSocket->Connect(STR::S2CS(httpgethost(httpworker)), httpgetPort(httpworker))){
			        memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			        aSocket->Receive((void *)szRecValue, 1024);
				    rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
				   // PrintRp(_T("link to ") + STR::S2CS(httpworker->gethost()));//rev
					PrintRp(_T("link to ") + STR::S2CS(httpgethost(httpworker)));//rev
				}
				else{
					CString error;
				    error.Format(_T("���ӷ�����ʧ��:%d"), aSocket->GetLastError());
				    PrintLog(error);
				    return false;
				}
			}
			//string data = httpworker->getMsg();
			string data = httpgetMsg(httpworker);
			aSocket->Send(data.c_str(), data.length());
			return true;
		}
		/**
         *@author  ACM2012
         *@return void
         *@note
         *the client receives a report from the server
         *@remarks the farmat of sending message is "FILE TYPE + " " + FILE LEN + " " + FILE Content"
		 */
		void Rev(){
			string html = "";
			char buf[BUFSIZE + 1];
			while (true){
				memset(buf, 0, sizeof(buf));
				aSocket->Receive(buf, BUFSIZE);
				buf[BUFSIZE] = '\0';
				html += (*new string(buf));
				if (html.find("\r\n\r\n")) break;
			}
			int xpos = html.find("\r\n\r\n");
			PrintRp(STR::S2CS(html.substr(0,xpos-1)));
			//int len = httpworker->setMsg(html);
			int len = httpsetmsg(httpworker, html);
			if (len > 0){//no data;
				html = httpgetdata(httpworker);//httpworker->getdata();
				int count;
				while (true){
					if (len == 0) break;
					memset(buf, 0, sizeof(buf));
					count = aSocket->Receive(buf, BUFSIZE);
					buf[BUFSIZE] = '\0';
					html += (*new string(buf));
					len -= count;
				}
				//httpworker->setdata(html);
				httpsetdata(httpworker,html);
			}
			//if (httpworker->analy()<0){
			if (httpanaly(httpworker)<0){
			}
			//Httpgetdata httpgetdata = (Httpgetdata)dll->GetFun("getdata");
			//PrintRp(STR::S2CS(httpworker->getdata()));
			PrintRp(STR::S2CS(httpgetdata(httpworker)));
		}
	
	private:
		NetDll *dll;
		//Httpworker *httpworker;
		void *httpworker;
		HttPtr httptr;
		HttpSetport httpsetport;
		Httpdiv httpdiv;
		HttpMake httpmake;
		HttpgetPort httpgetPort;
		Httpgethost httpgethost;
		HttpgetMsg httpgetMsg;
		HttpSetMsg httpsetmsg;
		Httpgetdata httpgetdata;
		Httpsetdata httpsetdata;
		Httpanaly httpanaly;

		DNSworker *dnsworker;
		CmySocket *aSocket;
		CString rev;
		TCHAR szRecValue[1024];
	};

}


/*
dnsworker = new DNSworker();
dnsworker->Make(httpworker->gethost());
string dnsmsg = dnsworker->getMsg();
CString temp = STR::S2CS(dnsmsg);
udp = new CSocket();
udp->Create( SOCK_DGRAM);
CString dns_host_ip = STR::S2CS(DNSworker::getdefault_dfs());
udp->SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), dns_port, dns_host_ip);
string dnsrmsg;
memset(szRecValue, 0, 1024 * sizeof(TCHAR));
CString s;
UINT t;
while (udp->ReceiveFromEx(szRecValue, 1024, s, t) == -1){
udp->SendToEx(temp, sizeof(TCHAR)*temp.GetLength(), dns_port, dns_host_ip);;
}
udp->Close();
delete udp;
temp.Format(_T("%s"), szRecValue);
dnsrmsg = STR::CS2S(temp);
dnsworker->setMsg(dnsrmsg);
if (dnsworker->Analy() < 0){
PrintRp(_T("host wrong"));
continue;
}
httpworker->setIP(dnsworker->Getip());
*/

/*
if (!httpworker->IPcheck()){//UDP ge IP
//UDP get IP accord to host
dnsworker = new DNSworker();
dnsworker->Make(httpworker->gethost());
string dnsmsg = dnsworker->getMsg();
CString temp = STR::S2CS(dnsmsg);
udp = new CSocket();
udp->Create();
CString dns_host_ip = STR::S2CS(DNSworker::getdefault_dfs());
if (!udp->Connect(dns_host_ip, dns_port)){ PrintRp(_T("dns server can't be access")); continue; };
udp->Send(temp, sizeof(TCHAR)*temp.GetLength());
string dnsrmsg;
memset(szRecValue, 0, 1024 * sizeof(TCHAR));
udp->Receive(szRecValue, 1024);
udp->Close();
delete udp;
temp.Format(_T("%s"), szRecValue);
dnsrmsg = STR::CS2S(temp);
dnsworker->setMsg(dnsrmsg);
if (dnsworker->Analy() < 0){
PrintRp(_T("host wrong"));
continue;
}
httpworker->setIP(dnsworker->Getip());
}
else httpworker->setIP(httpworker->gethost());
if (httpworker->getIP() == ""){
PrintRp(_T("host can't be analied"));
continue;
}*/

/*
rev = _T("");
CString temp;
int count;
while (true){
count = 0;
memset(szRecValue, 0, BUFSIZE * sizeof(TCHAR));
//
while (true){
count += aSocket->Receive((void *)(szRecValue + count), (BUFSIZE - count));
if (count == BUFSIZE) break;
}
temp.Format(_T("%s"), szRecValue);
string s = STR::RemoveStr(STR::CS2S(temp), '\n');
if (s[0] == '\0')
break;
rev += temp;
}
httpworker->setMsg(STR::CS2S(rev));
if (httpworker->analy() < 0){
PrintRp(_T("recieve wrong"));
return;
};
PrintRp(httpworker->show_rmsg());
*/

/*
CString data = STR::S2CS(httpworker->getMsg());
TCHAR *senddata;
int len = STR::CString2TCHAR(&senddata,&data);
int i;
for (i = 0; i < len; i += BUFSIZE){
if (i + BUFSIZE < len)
aSocket->Send(senddata + i, BUFSIZE*sizeof(TCHAR));
else{
CString str = senddata + i;
str += STR::FillStr(_T('\n'), BUFSIZE - (len % BUFSIZE));
aSocket->Send(str, BUFSIZE*sizeof(TCHAR));
}
}*/