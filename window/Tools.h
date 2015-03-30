//tools for other to use
#include <string>
//#include "afxinet.h"
typedef void* HMODULE;
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
	void AfxMessageBox(string Msg){
		AfxMessageBox(Msg);
	}
	//self make function 
	/*
	string Tchar2string(TCHAR *smsg){
		int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen*sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
		string str(chRtn);
		return str;
	}*/
	//Dllusr
	class Dlluser{
	public:
		//set the dll_path
		void Setpath(string path){
			this->path = path;
		}
		//set the function's name
		void Set_name(string name){
			this->fun_name = name;
		}
		//get the void* funtion
		void* Getfun(){
			FindName();
			return fun;
		}
	private:
		string path;
		string fun_name;
		HMODULE hmodele;
		void *fun;
		//find the name
		void FindName(){
			//fun = 

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