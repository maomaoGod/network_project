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
#include "Httpworker.h"
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
			PrintLog(_T("连接服务器成功"));
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
				error.Format(_T("创建失败:%d"), csocket.GetLastError());
				PrintLog(error);
				return;
			}
			/**@brief 转换需要连接的端口内容类型*/
			PrintLog(_T("套接字创建成功"));
			/**@brief 连接指定的地址和端口*/
			if (csocket.Connect(strIP, 6500))
			{
				CString rev;
				memset(szRecValue, 0, sizeof(szRecValue));
				/**brief send message to the server*/
				PrintLog(_T("连接服务器成功"));
				csocket.Send(Msg, Msg.GetLength()*sizeof(TCHAR));
				csocket.Receive((void *)szRecValue, 1024);
				rev.Format(_T("%s"), szRecValue);
			}
			else{
				CString error;
				error.Format(_T("创建失败:%d"), csocket.GetLastError());
				PrintLog(error);
			}
		}

	};

	class FTPWork{
	public:
		FTPWork(){
		}
		//div the word
		void split_input(CStringArray *data, CString Source, TCHAR div){
			//use the tools from namespace Tools
			STR::CCarg(data, Source, div);
		}
		void deal_send_Msg(CString *Msg, CStringArray *Data){
			int i;
			CString temp;
			for (i = 0; i < Data->GetSize(); i++){
				temp = Data->GetAt(i).Mid(0);
				*Msg = *Msg + _T(' ') + temp;
			}
		}
	};

	//take over the system
	class FTPApp{
	public:
		FTPApp(){
			AfxSocketInit();
			ftpwork = new FTPWork();
		}
		~FTPApp(){
			delete ftpwork;
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myftp>"));
			aSocket = new CSocket();
			//create a Socket
			if (!aSocket->Create()){
				CString error;
				error.Format(_T("创建失败:%d"), aSocket->GetLastError());
				PrintLog(error);
				return;
			}
			//take over the cmd
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				CStringArray code;
				ftpwork->split_input(&code, mystr, _T(' '));
				//Conn at first
				if (code[0] == _T("Conn")){
					PrintLog(code[1]);
					if (aSocket->Connect(code[1], 7600)){
						aSocket->Receive((void *)szRecValue, 1024);
						rev.Format(_T("来自服务器的消息:%s"), szRecValue);
						PrintRp(rev);
					}
					else{
						CString error;
						error.Format(_T("连接服务器失败:%d"), aSocket->GetLastError());
						PrintLog(error);
						return;
					}
				}
				//UPLOAD + client_path + serve_path
				else if (code[0] == _T("UPLOAD") || code[0] == _T("upload")){
					if (!Uploadfile(&code)){
						PrintLog(_T("Interrupt Error!"));
						continue;
					}
					Rev();
				}
				//DOWNLOAD + serve_path + client_path
				else if (code[0] == _T("DOWNLOAD") || code[0] == _T("download")){
					if (!Send(code[0], &code)){
						PrintLog(_T("Interrupt Error!"));
						continue;
					}
					Downloadfile(&code);
				}
				else{
					if (!Send(code[0], &code)){
						PrintLog(_T("Interrupt Error!"));
						continue;
					}
					Rev();
				}
			}
			aSocket->Close();
			delete aSocket;
		}
	private:
		FTPWork *ftpwork;
		CSocket *aSocket;
		CString IP;
		CString Path;
		CString rev;
		TCHAR szRecValue[1024];
		//UPLOAD + client_path + serve_path
		bool Uploadfile(CStringArray *data){
			string path = STR::CS2S(data->GetAt(1));
			string *temp;
			string read;
			char t[1024];
			CString Msg;
			FILE *fp;
			if (fopen_s(&fp, path.c_str(), "r")){
				PrintRp(_T("NONE FILE"));
				return false;
			}
			else{
				while (fscanf_s(fp, "%s", t, 1024) != -1){
					temp = new string(t);
					read += *temp;
					delete temp;
				}
			}
			Msg = data->GetAt(0) + _T(' ') + data->GetAt(2) + _T(' ') + STR::S2CS(read);
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		//DOWNLOAD + serve_path + client_path
		void Downloadfile(CStringArray *data){
			string path = STR::CS2S(data->GetAt(2));//client_path
			CString temp;
			FILE *fp;
			fopen_s(&fp, path.c_str(), "w");
			//while (){
			aSocket->Receive((void *)szRecValue, 1024);
			temp.Format(_T("%s"), szRecValue);
			//fprintf_s(fp, "%s", temp);
			fprintf_s(fp, "%s\n", STR::CS2S(temp).c_str());
			//}
			fclose(fp);
			PrintRp(_T("Rev OK"));
		}
		
		bool Send(CString Method, CStringArray *url){
			//memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			int i;
			CString Msg;
			ftpwork->deal_send_Msg(&Msg, url);
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		
		void Rev(){
			rev = _T("");
			CString temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			//while (aSocket->Receive((void *)szRecValue, 1024)){
			aSocket->Receive((void *)szRecValue, 1024);
			temp.Format(_T("%s"), szRecValue);
			rev += temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			//}
			PrintRp(rev);
		}

	};

	/**
	 *@class AppLayerHttp NetWork
	 *@brief How the Application Layer works
	 *@author   ACM2012
	 *@note
	 *to achieve communication of the client and the server with HTTP protocol
	 */

	class AppLayerHttp{
	public:
		AppLayerHttp(){
			AfxSocketInit();
			//httpwork = new HTTPWork();
			httpworker = new Httpworker();
			dnsworker = new DNSworker();
		}
		~AppLayerHttp(){
			//delete httpwork;
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
				if (httpworker->div(STR::CS2S(mystr), ' ') < 0){
					PrintLog(_T("argc error"));
					continue;
				}
				aSocket = new CSocket();
			    if (!aSocket->Create()){
					CString error;
				    error.Format(_T("创建失败:%d"), aSocket->GetLastError());
				    PrintLog(error);
				    return;
			    }
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
			httpworker->Make();
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			if (aSocket->Connect(STR::S2CS(httpworker->getIP()), HTTP_PORT)){
				aSocket->Receive((void *)szRecValue, 1024);
				rev.Format(_T("来自服务器的消息:%s"), szRecValue);
				PrintRp(rev);
			}
			else{
				CString error;
				error.Format(_T("连接服务器失败:%d"), aSocket->GetLastError());
				PrintLog(error);
				return false;
			}
			CString data = STR::S2CS(httpworker->getMsg());
			aSocket->Send(data, data.GetLength()*sizeof(TCHAR));
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
			rev = _T("");
			CString temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			while (aSocket->Receive((void *)szRecValue, 1024)){
				temp.Format(_T("%s"), szRecValue);
				rev += temp;
				memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			}
			httpworker->setMsg(STR::CS2S(rev));
			if (httpworker->analy() < 0){
				PrintRp(_T("recieve wrong"));
				return;
			};
			PrintRp(httpworker->show_rmsg());
		}
	
	private:
		Httpworker *httpworker;
		DNSworker *dnsworker;
		CSocket *aSocket;
		CSocket *udp;
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
