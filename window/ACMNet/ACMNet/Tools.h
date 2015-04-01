//tools for other to use
#include <string>
#include <windows.h>
//#include "afxinet.h"
/*typedef void* PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;*/
//type of funciton
//typedef int(*Int2Int)(int nVal1, int nval2);
#pragma once
using namespace std;

namespace Tools{
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
		void SetUrl(string url){
			this->url = url;
		}
		string GetText(){
			return this->document;
		}
		bool HtmlAsk(){
			this->finish = false;
			this->success = false;
			Request();
			while (true){
				if (this->finish) return false;
			}
			return this->success;
		}
	private:
		string url; // the url Msg
		string document; //the html Msg
		bool finish; //finish is tell us how to work on it
		bool success;
		// the function to post/get
	    void Request(){
			//set the document acconding to the path
			document = "";
			try{
				//socket
			}
			catch(exception e){
				this->finish = true;
				this->success = false;
			}
			this->finish = true;
			this->success = true;
		}
	};
}