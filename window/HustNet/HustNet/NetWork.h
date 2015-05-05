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

	class FTPWork{
	public:
		FTPWork(){
			AfxSocketInit();
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myftp>"));
			aSocket = new CSocket();
			if (!aSocket->Create()){
				CString error;
				error.Format(_T("����ʧ��:%d"), aSocket->GetLastError());
				PrintLog(error);
				return;
			}
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				CStringArray code;
				Tstr::CCarg(&code, mystr, _T(' '));
				if (code[0] == _T("Conn")){
					if (aSocket->Connect(IP, 7600)){
						aSocket->Receive((void *)szRecValue, 1024);
						rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
						PrintRp(rev);
					}
					else{
						CString error;
						error.Format(_T("���ӷ�����ʧ��:%d"), aSocket->GetLastError());
						PrintLog(error);
						return;
					}
				}
				//UPLOAD + client_path + serve_path
				else if (code[0] == _T("UPLOAD") || code[0] == _T("upload")){
					if (Uploadfile(&code)){
						PrintLog(_T("Interrupt Error!"));
					}
					else Rev();
				}
				//DOWNLOAD + serve_path + client_path
				else if (code[0] == _T("DOWNLOAD") || code[0] == _T("download")){
					if (!Send(code[0], &code)){
						PrintLog(_T("Interrupt Error!"));
					}
					else Downloadfile(&code);
				}
				else{
					if (!Send(code[0], &code)){
						PrintLog(_T("Interrupt Error!"));
					}
					else Rev();
				}
			}
			aSocket->Close();
			delete aSocket;
		}
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
			Msg = data->GetAt(0) + _T(' ') + Tstr::S2CS(read) + _T(' ') + data->GetAt(2);
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		//DOWNLOAD + serve_path + client_path
		void Downloadfile(CStringArray *data){
			string path = Tstr::CS2S(data->GetAt(2));//client_path
			CString temp;
			FILE *fp;
			fopen_s(&fp, path.c_str(), "w");
			while (aSocket->Receive((void *)szRecValue, 1024)){
				temp.Format(_T("%s"), szRecValue);
				fprintf_s(fp, "%s\n", temp);
			}
			PrintRp(_T("Rev OK"));
		}
		bool Send(CString Method, CStringArray *url){
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			int i;
			CString Msg, temp;
			Msg = Method + _T(' ') + Path;
			for (i = 1; i < url->GetSize(); i++){
				temp = url->GetAt(i).Mid(0);
				Msg = Msg + _T(' ') + temp;
			}
			aSocket->Send(Msg, Msg.GetLength()*sizeof(TCHAR));
			return true;
		}
		void Rev(){
			rev = _T("");
			CString temp;
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			while (aSocket->Receive((void *)szRecValue, 1024)){
				temp.Format(_T("%s"), szRecValue);
				rev += temp;
				memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			}
			PrintRp(rev);
		}

	private:
		CSocket *aSocket;
		CString IP;
		CString Path;
		CString rev;
		TCHAR szRecValue[1024];
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
		}
		
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Myhttp>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				CleanRp(NULL);
				PrintLog(_T("Accept ") + mystr);
				CStringArray code;
				Tstr::CCarg(&code ,mystr ,_T(' '));
				//Create();
				aSocket = new CSocket();
				//failed
			    if (!aSocket->Create()){
					CString error;
				    error.Format(_T("����ʧ��:%d"), aSocket->GetLastError());
				    PrintLog(error);
				    return;
			    }
				//argc size
				if (code.GetSize() < 2) {
					PrintLog(_T("��������"));
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
			int i = url->GetAt(1).Find(_T('/'));
			//int i = url.Find(_T('\\'));
			IP = url->GetAt(1).Mid(0, i);
			Path = url->GetAt(1).Mid(i + 1);//
			PrintLog(IP);
			PrintLog(Path);
			if(aSocket->Connect(IP, 6500)){
				aSocket->Receive((void *)szRecValue, 1024);
				rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
				PrintRp(rev);
			}
			else{
				CString error;
				error.Format(_T("���ӷ�����ʧ��:%d"), aSocket->GetLastError());
				PrintLog(error);
				return false;
			}
			CString Msg, temp;
			Msg = Method + _T(' ') + Path;
			for (i = 2; i < url->GetSize(); i++){
				temp = url->GetAt(i).Mid(0);
				Msg = Msg + _T(' ') + temp;
			}
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
		CSocket *aSocket;
		CString IP;
		CString Path;
		CString rev;
		TCHAR szRecValue[1024];
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
		    	//pMsg->AddRecipient(res[0], rens[0]��"");
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
				pMsg->Send("wangluokecs2012:111111z@smtp��163.com", VARIANT_FALSE);
			}
			catch(_com_error e){
				PrintLog(e.ErrorMessage());
				PrintRp(_T("ȷ������ȷ��װ��JHttp���"));
			}
		}
	};

	class FTPWork{

	};
}
*/
