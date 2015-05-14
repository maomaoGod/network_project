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
//#import "dll/JHttp.dll"

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
				Tstr::CCarg(&cmd, mystr, _T(' '));
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
			Tstr::CCarg(data, Source, div);
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
			string path = Tstr::CS2S(data->GetAt(1));
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
			Msg = data->GetAt(0) + _T(' ') + data->GetAt(2) + _T(' ') + Tstr::S2CS(read);
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		//DOWNLOAD + serve_path + client_path
		void Downloadfile(CStringArray *data){
			string path = Tstr::CS2S(data->GetAt(2));//client_path 
			CString temp;
			FILE *fp;
			fopen_s(&fp, path.c_str(), "w");
			//while (){
			aSocket->Receive((void *)szRecValue, 1024);
			temp.Format(_T("%s"), szRecValue);
			//fprintf_s(fp, "%s", temp);
			fprintf_s(fp, "%s\n", Tstr::CS2S(temp).c_str());
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

	class HTTPWork{
	public:
		HTTPWork(){
		}
		//div the word
		void split_input(CStringArray *data, CString Source, TCHAR div){
			//use the tools from namespace Tools
			Tstr::CCarg(data, Source, div);
		}
		void DivP(CStringArray *Data){
			int i = Data->GetAt(1).Find(_T('/'));
			IP = Data->GetAt(1).Mid(0, i);
			Path = Data->GetAt(1).Mid(i + 1);//
		}
		CString GetIP(){
			return IP;
		}
		CString GetPath(){
			return Path;
		}
		void deal_send_Msg(CString *Msg, CStringArray *Data){
			int i; 
			CString temp;
			*Msg = Data->GetAt(0) + _T(' ') + Path;
			for (i = 2; i < Data->GetSize(); i++){
				temp = Data->GetAt(i).Mid(0);
				*Msg = *Msg + _T(' ') + temp;
			}
		}
	private:
		CString IP;
		CString Path;
	};

	class AppLayerHttp{
	public:
		AppLayerHttp(){
			AfxSocketInit();
			httpwork = new HTTPWork();
		}
		~AppLayerHttp(){
			delete httpwork;
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myhttp>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				CStringArray code;
				httpwork->split_input(&code ,mystr ,_T(' '));
				//Create();
				aSocket = new CSocket();
				//failed
			    if (!aSocket->Create()){
					CString error;
				    error.Format(_T("创建失败:%d"), aSocket->GetLastError());
				    PrintLog(error);
				    return;
			    }
				//argc size
				if (code.GetSize() < 2) {
					PrintLog(_T("参数过少"));
					return;
				}
				//send
				if (!Send(code[0], &code)){
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
		bool Send(CString Method, CStringArray *url){
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			//int i = url.Find(_T('\\'));
			httpwork->DivP(url);
			if(aSocket->Connect(httpwork->GetIP(), 6500)){
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
			CString Msg;
			httpwork->deal_send_Msg(&Msg, url);
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
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
			//1. get save type
			//2. get file len
			//3. define Cache[1024]
			//4. get all data by recive case by case
			//5. turn it to the file type
			//6. show it
			rev = _T("");
			CString temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			while (aSocket->Receive((void *)szRecValue, 1024)){
				temp.Format(_T("%s"),szRecValue);
				rev += temp;
				memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			}
			PrintRp(rev);
		}
	
	private:
		//typedef void(AppLayerHttp::*DealWithFunciton)(vector<CString> data);
		//no data
		HTTPWork *httpwork;
		CSocket *aSocket;
		//CString IP;
		//CString Path;
		CString rev;
		TCHAR szRecValue[1024];
	};

	class SMTPWork
	{
	public:
		SMTPWork()
		{
			command.clear();
			command[_T("auth")] = 1;
			command[_T("helo")] = 1;
			command[_T("mail")] = 1;
			command[_T("rcpt")] = 1;
			command[_T("noop")] = 1;
			command[_T("help")] = 1;
			command[_T("data")] = 1;
			command[_T("quit")] = 1;
			command[_T("rset")] = 1;

		}
		bool checkgrammar(CString s)
		{
			s.MakeLower();
			if (command[s])
				return true;
			else
				return false;
		}
		//div the word
		void split_input(CStringArray *data, CString Source, TCHAR div){
			//use the tools from namespace Tools
			Tstr::CCarg(data, Source, div);
		}
	private:
		map <CString, bool> command;
	};
#define RECEIVE_CMD 0
#define RECEIVE_MAIL 1
#define SMTP_PORT 8000
	class AppLayerSMTP
	{
	public:
		AppLayerSMTP(CString IP){
			AfxSocketInit();
			smtpwork = new SMTPWork();
			aSocket = new CSocket();
			if (!aSocket->Create()){
				CString error;
				error.Format(_T("创建失败:%d"), aSocket->GetLastError());
				PrintLog(error);
				return;
			}
			TCHAR szRecValue[1024] = { 0 };
			CString rev;
			if (aSocket->Connect(IP, 8000)){
				aSocket->Receive((void *)szRecValue, 1024);
				rev.Format(_T("来自服务器的消息:%s"), szRecValue);
				PrintRp(rev);
				Connect = true;
			}
			else{
				CString error;
				error.Format(_T("连接服务器失败:%d"), aSocket->GetLastError());
				PrintLog(error);
				Connect = false;
			}
			state = RECEIVE_CMD;
		}
		~AppLayerSMTP(){
			delete smtpwork;
			delete aSocket;
		}
	public:
		void Begin(){
			CString mystr;

			if (!Connect)
				return;
			bool finish = 0;
			TakeOverCmd(_T("MySMTP>"));
			do
			{
				mystr = GetLine();
				CleanRp(NULL);
				if (state == RECEIVE_CMD)
				{

					CStringArray code;
					smtpwork->split_input(&code, mystr, _T(' '));
					if (!smtpwork->checkgrammar(code[0]))
					{
						PrintLog(_T("Unknown Command!\n"));
					}
					else
					{
						if (code[0].MakeLower() == _T("quit"))
							finish = 1;
						PrintLog(_T("Accept: ") + code[0]);
						if (!Send(mystr))
						{
							PrintLog(_T("Interrupt Error!"));
							return;
						}
						CString Revdata = Rev();
						CStringArray rev_split;
						smtpwork->split_input(&rev_split, Revdata, _T('\r'));
						if (code[0].MakeLower() == _T("data") && rev_split[0] == "354")
							state = RECEIVE_MAIL;
						PrintRp(Revdata);
					}
				}
				else
				{
					if (!Send(mystr))
					{
						PrintLog(_T("Interrupt Error!"));
						return;
					}
					if (mystr == _T("."))
					{
						CString Revdata = Rev();
						PrintRp(Revdata);
						state = RECEIVE_CMD;
					}
				}
			} while (!finish);
		}
	private:
		bool Send(CString Msg)
		{
			if (!aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR)))
				return false;
			return true;
		}
		CString Rev()
		{
			CString temp;
			temp = _T("");
			TCHAR RecBuffer[1024] = { 0 };
			if (aSocket->Receive((void *)RecBuffer, 1024)){
				CString t;
				t.Format(_T("%s"), RecBuffer);
				temp += t;
				memset(RecBuffer, 0, 1024 * sizeof(TCHAR));
			}
			return temp;
		}
	private:
		SMTPWork *smtpwork;
		CSocket *aSocket;
		bool Connect;
		int state;
	};

}


/*
=======
>>>>>>> parent of 0a428e0... the Http on Serve
	//send a Http
	class HttpSend // use the
	{
	public:
		HttpSend(){
			init();
			CoInitialize(NULL);
			UserName = _T("wangluokecs2012@163.com");
			PassWord = _T("111111z");
			SendAddr = _T("smtp.163.com");
			Subject = _T("Hello World\n");
			MsgText = _T("World Beautiful\n");
			RevAddr = _T("authwork2@163.com");
		}
		~HttpSend(){
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Http>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				PrintLog(_T("Accept ") + mystr);
				switch (HttpCmd[mystr]){
				case 0: break;
				case 1: break;
				case 2: break;
				case 3: break;
				case 4:
					SendHttp();
					break;
				case 5: SetRev(mystr); PrintRp(_T("set to send to ") + this->RevAddr); break;
				default: PrintLog(_T("Error Code"));
				}
			}
		}
		void SetRev(CString Addr){ this->RevAddr = Addr; }
		void SetSub(CString Sub){ this->Subject = Sub; }
		void SetMsg(CString Msg){ this->MsgText = Msg; }
	private:
		map<CString, int> HttpCmd;
		CString UserName, PassWord;
		CString RevAddr, SendAddr;
		CString Subject, MsgText;
		//stmp name passw
		//sender
		//http://hi.baidu.com/alalmn/item/48c2e0d33e959494260ae7a5

		void init(){
			HttpCmd[_T("exit")] = 0;//exit
			HttpCmd[_T("user")] = HttpCmd[_T("User")] = 1;//u
			HttpCmd[_T("password")] = HttpCmd[_T("Password")] = 2;//pw
			HttpCmd[_T("STMP")] = 3;//set stmp
			HttpCmd[_T("Send")] = HttpCmd[_T("send")] = 4;//send Http
			HttpCmd[_T("To")] = HttpCmd[_T("to")] = 5;//to someone
		}
		void SendHttp(){
			try{
			    jHttp::IMessagePtr pMsg("JHttp.Message");//waitjHttp::Message
		     	pMsg->Priority = 3;
		    	pMsg->Charset = "GB2312";
				pMsg->From = "wangluokecs2012@163.com";//Tstr::CS2S(UserName).c_str();
				pMsg->FromName = "wlk";//Tstr::CS2S(UserName).c_str();
		    	//const char *res[] = {Tstr::CS2S(RevAddr).c_str()};
		    	//const char *rens[] = {Tstr::CS2S(RevAddr).c_str()};
		    	//pMsg->AddRecipient(res[0], rens[0]，"");
	     		//pMsg->AddRecipient(res[0], rens[0], "");authwork2@163.com
				pMsg->AddRecipient("834658870@qq.com", "", "");//authwork2@163.com"834658870@qq.com"
				pMsg->Subject = "abcd";//Tstr::CS2S(Subject).c_str();
				pMsg->Body = "123456789";//Tstr::CS2S(MsgText).c_str();
		    	pMsg->AppendText("You can add any words when you want\n");
				//pMsg->HttpServerUserName = "wangluokecs2012@163.com";//Tstr::CS2S(UserName).c_str();
				//pMsg->HttpServerPassWord = "111111z";// Tstr::CS2S(PassWord).c_str();
				//CString SendInfo = _T("");
		    	//SendInfo += UserName;
				//SendInfo += _T(":");
				//SendInfo += UserName;
		    	//SendInfo += PassWord;
		    	//SendInfo += _T("@");
		    	//SendInfo += SendAddr;
				//_bstr_t(SendInfo)
				pMsg->Send("wangluokecs2012:111111z@smtp。163.com", VARIANT_FALSE);
			}
			catch(_com_error e){
				PrintLog(e.ErrorMessage());
				PrintRp(_T("确保你正确安装了JHttp组件"));
			}
		}
	};

	class FTPWork{

	};
}
*/
