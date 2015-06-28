//tools for other to use
#pragma once
#include <string>
//#include <windows.h>
#include <map>
#include <vector>
#include <mapi.h>
#include "afxinet.h"

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

		static void Split(string Seq, vector<string> *strArray, char div){
			if (strArray->size() != 0){
				strArray->clear();
			}
			string substring;
			int i, j;
			int len = Seq.length();
			for (i = 0; i < len; i++){
				for (j = i; j < len; j++){
					if (Seq[j] == div) break;
				}
				/**<for a substring is*/
				if (j > i)substring = Seq.substr(i, j - i);
				strArray->push_back(substring);
				i = j;
			}
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
				htmlFile = (CHttpFile*)mySession.OpenURL(this->url);//�������ӻ�ȡ����;
				while (htmlFile->ReadString(src, 1024)){//��������ÿһ��
					int nBufferSize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, NULL, 0);
					TCHAR *pBuffer = new TCHAR[nBufferSize + 1];
					memset(pBuffer, 0, (nBufferSize + 1)*sizeof(TCHAR));
					MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)src, -1, pBuffer, nBufferSize*sizeof(TCHAR));//UTF-8תUnicode
					document += pBuffer;
					document += _T("\r\n");
					delete pBuffer;
				}
				htmlFile->Close();
				mySession.Close(); //�رջỰ
			}
			catch (CException *e) {  //�쳣����
				this->finish = true;
				this->success = false;
				this->Log = _T("��ȡ����ʧ��" );//+ e->
				
				//AfxMessageBox(_T("��ȡ����ʧ��"));
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
iMapiInstalled = GetProfileInt(_T("Http"), _T("MAPI"), 0);
if (!iMapiInstalled) return false;//failed
HttpHelp.Setpath(_T("MAPI32.DLL"));
if(!HttpHelp.Load_dll()) return false;//failed load
HttpHelp.Set_fun_name("MAPILogon");
pMAPILogon = (LPMAPILOGON)HttpHelp.Getfun();
HttpHelp.Set_fun_name("MAPILogoff");
pMAPILogoff = (LPMAPILOGOFF)HttpHelp.Getfun();
HttpHelp.Set_fun_name("MAPISendHttp");
pMAPISendHttp = (LPMAPISENDHttp)HttpHelp.Getfun();
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

	Result = pMAPISendHttp(lhSession, 0, &Msg, 0, 0);
	if (Result != SUCCESS_SUCCESS){
		PrintLog(_T("Error for send a Http"));
		return false;
	}
	pMAPILogoff(lhSession, 0, 0, 0);
}
HttpHelp.free_dll();
return 1;
		}

*/




