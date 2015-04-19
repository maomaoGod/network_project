/**@file
 *@brief sending and receiving message from server
 *@author 
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *As to send message, the format is CMD + " " + PATH + " " + HOST
 *As to receive message, the format is FILE TYPE + " " + FILE LEN + " " + FILE CONTENT
 */

#pragma once
#include "stdafx.h"
#include "map"
#include "Tools.h"
//#import "dll/Jmail.dll"

extern void PrintLog(CString);
extern void PrintRp(CString);
extern void CleanLog();
extern void CleanRp();
extern void SetRp(CString);
extern void TakeOverCmd(CString e);
extern void SetHint(CString e);
extern CString GetLine();

using namespace std;
using namespace Tools;

namespace NetWork{
	/**
	 *@class ChatWork NetWork
	 *@brief Socket
	 *@author 
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
		/**< CString Msg*/
		CString Obj;
		CString CNo, TNo;
		CSocket csocket;
		/**< IP*/
		CString strIP;
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

	};

	/**
	 *@class AppLayerHttp NetWork
	 *@brief How the Application Layer works
	 *@author 
	 *@note
	 *to achieve communication of the client and the server with HTTP protocol
	 */
	class AppLayerHttp{
	public:
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Mail>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				PrintLog(_T("Accept ") + mystr);
				switch (0){
				case 0: break;
				case 1: break;
				case 2: break;
				case 3: break;
				case 4: break;
				case 5: break;
				default: PrintLog(_T("Error Code"));
				}
			}
		}
		/**
         *@brief the client sends a report to the server
         *@author
         *@param in Method HTTP CMD
         *@param in url the link which waiting to access
         *@exception XXXXXXX
         *@return True if successfully, otherwise False
         *@note
         *XXXXXXXX
         *@remarks the format of sending message is "CMD + " " + PATH + " " + HOST"
		 */
		bool Send(string Method, string url){
			//1 div url to path + IP
			//2 com Method + " " + path + IP
			//3 send com Msg
			//return true if TCP finish
			//else return false
		}

		/**
         *@brief the client receives a report from the server
         *@author
         *@param XXXXXXX
         *@param XXXXX
         *@exception XXXXXXX
         *@return void
         *@note
         *XXXXXXXX
         *@remarks the farmat of sending message is "FILE TYPE + " " + FILE LEN + " " + FILE Content"
		 */
		void Rev(){
			//1. get save type
			//2. get file len
			//3. define Cache[1024]
			//4. get all data by recive case by case
			//5. turn it to the file type
			//6. show it
		}
	private:
		//no data
	};
}


/*
=======
>>>>>>> parent of 0a428e0... the Http on Serve
	//send a mail
	class MailSend // use the
	{
	public:
		MailSend(){
			init();
			CoInitialize(NULL);
			UserName = _T("wangluokecs2012@163.com");
			PassWord = _T("111111z");
			SendAddr = _T("smtp.163.com");
			Subject = _T("Hello World\n");
			MsgText = _T("World Beautiful\n");
			RevAddr = _T("authwork2@163.com");
		}
		~MailSend(){
		}
		void Begin(){
			CString mystr;
			TakeOverCmd(_T("Mail>"));
			while ((mystr = GetLine()).Compare(_T("exit")) != 0){
				PrintLog(_T("Accept ") + mystr);
				switch (MailCmd[mystr]){
				case 0: break;
				case 1: break;
				case 2: break;
				case 3: break;
				case 4:
					SendMail();
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
		map<CString, int> MailCmd;
		CString UserName, PassWord;
		CString RevAddr, SendAddr;
		CString Subject, MsgText;
		//stmp name passw
		//sender
		//http://hi.baidu.com/alalmn/item/48c2e0d33e959494260ae7a5

		void init(){
			MailCmd[_T("exit")] = 0;//exit
			MailCmd[_T("user")] = MailCmd[_T("User")] = 1;//u
			MailCmd[_T("password")] = MailCmd[_T("Password")] = 2;//pw
			MailCmd[_T("STMP")] = 3;//set stmp
			MailCmd[_T("Send")] = MailCmd[_T("send")] = 4;//send mail
			MailCmd[_T("To")] = MailCmd[_T("to")] = 5;//to someone
		}
		void SendMail(){
			try{
			    jmail::IMessagePtr pMsg("JMail.Message");//waitjmail::Message
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
				//pMsg->MailServerUserName = "wangluokecs2012@163.com";//Tstr::CS2S(UserName).c_str();
				//pMsg->MailServerPassWord = "111111z";// Tstr::CS2S(PassWord).c_str();
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
				PrintRp(_T("确保你正确安装了Jmail组件"));
			}
		}
	};

	class FTPWork{

	};
}
*/
