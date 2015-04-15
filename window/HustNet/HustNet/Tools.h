//tools for other to use
#pragma once
#include <string>
//#include <windows.h>
#include <map>
#include <mapi.h>
#include "afxinet.h"

extern void PrintLog(CString);
extern void PrintRp(CString);
extern void CleanLog();
extern void CleanRp();
extern void SetRp(CString);
extern void TakeOverCmd(CString e);
extern void SetHint(CString e);
extern CString GetLine();

using namespace std;

namespace Tools{
	//self make function 
	// tools for Tchar to string
	//
	class Tstr{
	public:
		//Tchar to string
		static string Tchar2string(TCHAR *smsg){
				int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
				char* chRtn = new char[iLen*sizeof(char)];
				WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
				string str(chRtn);
				return str;
			}
		// ANSI To Unicode
		static wstring ANSIToUnicode(const string& str)
		{
			int len = 0;
			len = str.length();
			int  unicodeLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
			wchar_t *  pUnicode;
			pUnicode = new  wchar_t[unicodeLen + 1];
			memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
			::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, (LPWSTR)pUnicode, unicodeLen);
			wstring  rt;
			rt = (wchar_t*)pUnicode;
			delete  pUnicode;
			return  rt;
		}
		//string to LPCWSTR
		static LPCWSTR String2LPCWSTR(string str){
			/*wstring w_str = ANSIToUnicode(str);
			LPCWSTR L_str = w_str.c_str();
			return L_str;*/
			size_t origsize = str.length() + 1;
			const size_t newsize = 100;
			size_t convertedChars = 0;
			wchar_t *wcstring = (wchar_t *)malloc(sizeof(wchar_t)*(str.length() - 1));
			mbstowcs_s(&convertedChars, wcstring, origsize, str.c_str(), _TRUNCATE);
			return wcstring;
		}

		//Csting to LPSTR
		static LPSTR CString2LPSTR(CString Subject){
			LPSTR sub = (LPSTR)Subject.GetBuffer();
			Subject.ReleaseBuffer();
			return sub;
		}

		static string CS2S(CString Subject){
			string sub = CString2LPSTR(Subject);//Subject.GetBuffer(0);
			return sub;
		}

		static char *S2Cstar(string Subject)
		{
			int len = Subject.length() + 1, i;
			char *temp = (char *)malloc(len*sizeof(char));
			for (i = 0; i < Subject.length(); ++i)
			{
				temp[i] = Subject[i];
			}
			temp[i] = '\0';
			return temp;
		}

		//command & args
		
		static void CCarg(CStringArray *strArry, CString Cmd, TCHAR div){
			int i = 0;
			CString strGet(_T(""));
			//CStringArray strArry;
			while (AfxExtractSubString(strGet, Cmd, i++, div)){
				strArry->Add(strGet);
			}
			//return strArry;
		}

	};
	//Dllusr
	class Dlluser{
	public:
		//set the dll_path
		void Setpath(CString path){
			this->path = path;
		}
		//set the function's name
		void Set_fun_name(string name){
			this->fun_name = name;
		}
		//load dll
		bool Load_dll(){
			hmodele = LoadLibrary(this->path);//()this->path.c_str()
			if (!hmodele) return false;
			return true;
		}
		//get the void* funtion
		void* Getfun(){
			Find_function();
			return fun;
		}
		void free_dll(){
			FreeLibrary(hmodele);
		}
	private:
		CString path;
		string fun_name;
		HMODULE hmodele;
		void *fun;
		//find the function
		void Find_function(){
			//fun = 
			if (fun_name == "") { fun = NULL; return ; }
			try{
				fun = GetProcAddress(hmodele, this->fun_name.c_str());//
			}catch (exception e){
				fun = NULL;
				return ;
			}
			return ; 
		}
	};
	//get html 
	class HtmlMsg{
    // the function used by public
	public:
		HtmlMsg(){
		}
		HtmlMsg(LPCTSTR url){
			this->url = url;
		}
		~ HtmlMsg(){
		}
		void SetUrl(LPCTSTR url){
			this->url = url;
		}
		CString GetText(){
			return this->document;
		}
		CString GetLog(){
			return this->Log;
		}
		bool HtmlAsk(){
			this->finish = false;
			this->success = false;
			getURLContext();
			while (true){
				if (this->finish) break;
			}
			return this->success;
		}
	private:
		LPCTSTR url; // the url Msg
		CString document; //the html Msg
		CString Log;
		bool finish; //finish is tell us how to work on it
		bool success;
		// the function to get htmlMsg
		void getURLContext(){
			document = _T("");
			CInternetSession mySession(NULL, 0);//set session
			CHttpFile* htmlFile = NULL; //http file
			TCHAR src[1024];
			try{
				htmlFile = (CHttpFile*)mySession.OpenURL(this->url);//建立连接获取输入;
				while (htmlFile->ReadString(src, 1024)){//处理流中每一行
					int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0);
					TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
					memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
					MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8转Unicode
					document += pBuffer;
					document += _T("\r\n");
					delete pBuffer;
				}
				htmlFile->Close();
				mySession.Close(); //关闭会话
			}
			catch (CException *e) {  //异常处理
				this->finish = true;
				this->success = false;
				this->Log = _T("获取数据失败" );//+ e->
				
				//AfxMessageBox(_T("获取数据失败"));
			}
			this->finish = true;
			this->success = true;
		};
	};
	
	

}


//overload : it don't need to use _T anymore
/*void AfxMessageBox(string Msg){
AfxMessageBox(Msg);
}

void AfxMessageBox(string Msg, UINT BS, UINT IS){//
AfxMessageBox(Msg, BS, IS);//,
}
int AfxMessageBox(string Msg, UINT BS, UINT IS){//
return AfxMessageBox(Msg, BS, IS);//,
}*/

/*
/*typedef void* PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;*/
//type of funciton
//typedef int(*Int2Int)(int nVal1, int nval2);

/*
string Tchar2string(TCHAR *smsg){
int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
char* chRtn = new char[iLen*sizeof(char)];
WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
string str(chRtn);
return str;
}
// ANSI To Unicode
wstring ANSIToUnicode(const string& str)
{
int len = 0;
len = str.length();
int  unicodeLen = ::MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,NULL,0);
wchar_t *  pUnicode;
pUnicode = new  wchar_t[unicodeLen + 1];
memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
::MultiByteToWideChar(CP_ACP,0,str.c_str(),-1,(LPWSTR)pUnicode,unicodeLen);
wstring  rt;
rt = (wchar_t*)pUnicode;
delete  pUnicode;
return  rt;
}
//string to LPCWSTR
LPCWSTR Sting2LPCWSTR(string str){
wstring w_str = ANSIToUnicode(str);
LPCWSTR L_str = w_str.c_str();
return L_str;
}*/
/*
bool Repared(){
iMapiInstalled = GetProfileInt(_T("Mail"), _T("MAPI"), 0);
if (!iMapiInstalled) return false;//failed
MailHelp.Setpath(_T("MAPI32.DLL"));
if(!MailHelp.Load_dll()) return false;//failed load
MailHelp.Set_fun_name("MAPILogon");
pMAPILogon = (LPMAPILOGON)MailHelp.Getfun();
MailHelp.Set_fun_name("MAPILogoff");
pMAPILogoff = (LPMAPILOGOFF)MailHelp.Getfun();
MailHelp.Set_fun_name("MAPISendMail");
pMAPISendMail = (LPMAPISENDMAIL)MailHelp.Getfun();
//to log
LPSTR name = (LPSTR)UserName.GetBuffer();
UserName.ReleaseBuffer();
LPSTR pass = (LPSTR)PassWord.GetBuffer();
PassWord.ReleaseBuffer();
/*if (pMAPILogon(0, name, pass, MAPI_LOGON_UI, 0, &lhSession) != SUCCESS_SUCCESS){
PrintLog(_T("Error user or password!"));
return false;
}*/
/*
if (pMAPILogon(0, NULL, NULL, MAPI_LOGON_UI, 0, &lhSession) != SUCCESS_SUCCESS){
	PrintLog(_T("Error user or password!"));
	return false;
}
else{
	ULONG Result;
	MapiMessage Msg;
	MapiRecipDesc Recer[1];
	Recer[0].ulRecipClass = MAPI_TO;
	LPSTR sendname = (LPSTR)RevAddr.GetBuffer();
	RevAddr.ReleaseBuffer();
	Recer[0].lpszName = sendname;
	Recer[0].lpszAddress = sendname;
	Recer[0].ulEIDSize = 0;
	Recer[0].lpEntryID = 0;
	//set Msg
	memset(&Msg, 0, sizeof(Msg));
	LPSTR sub = (LPSTR)Subject.GetBuffer();
	Subject.ReleaseBuffer();
	Msg.lpszSubject = sub;
	LPSTR text = (LPSTR)MsgText.GetBuffer();
	MsgText.ReleaseBuffer();
	Msg.lpszNoteText = text;
	Msg.nRecipCount = 1;
	Msg.lpRecips = Recer;

	Result = pMAPISendMail(lhSession, 0, &Msg, 0, 0);
	if (Result != SUCCESS_SUCCESS){
		PrintLog(_T("Error for send a mail"));
		return false;
	}
	pMAPILogoff(lhSession, 0, 0, 0);
}
MailHelp.free_dll();
return 1;
		}

*/




