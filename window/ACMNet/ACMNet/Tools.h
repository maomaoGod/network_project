//tools for other to use
#include <string>
#include <windows.h>
#include "afxinet.h"
/*typedef void* PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;*/
//type of funciton
//typedef int(*Int2Int)(int nVal1, int nval2);
#pragma once
using namespace std;

namespace Tools{
	//self make function 
	// tools for Tchar to string
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
	}
	//Dllusr
	class Dlluser{
	public:
		//set the dll_path
		void Setpath(string path){
			this->path = path;
		}
		//set the function's name
		void Set_fun_name(string name){
			this->fun_name = name;
		}
		//load dll
		bool Load_dll(){
			hmodele = LoadLibrary(Sting2LPCWSTR(this->path));//()this->path.c_str()
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
		string path;
		string fun_name;
		HMODULE hmodele;
		void *fun;
		//find the function
		void Find_function(){
			//fun = 
			if (fun_name == "") { fun = NULL; return ; }
			try{
				fun = GetProcAddress(hmodele, this->fun_name.c_str());
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
				this->Log = _T("获取数据失败");
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