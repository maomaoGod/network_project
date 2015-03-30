//tools for other to use
#include <cstring>
#include <thread>
#include <windows.h>
#include "CWebBrowser2.h"
#include "afxinet.h"
#include "stdafx.h"
#include "afxhtml.H "
#pragma once
using namespace std;
namespace Tools{
	//overload : it don't need to use _T anymore
	void AfxMessageBox(string Msg){
		AfxMessageBox(Msg);
	}
	//self make function 
	string Tchar2string(TCHAR *smsg){
		int iLen = WideCharToMultiByte(CP_ACP, 0, smsg, -1, NULL, 0, NULL, NULL);
		char* chRtn = new char[iLen*sizeof(char)];
		WideCharToMultiByte(CP_ACP, 0, smsg, -1, chRtn, iLen, NULL, NULL);
		string str(chRtn);
		return str;
	}

	//Dllusr
	class Dlluser{


	public:
		void Getpath(string path){
			this->path = path;
		}

	private:
		string path;
	};

	//get html 
	class Htmlget{
	public:
		void GetUrl(LPCTSTR url){
			this->url = url;
		}
		string GetText(){
			return this->document;
		}
		bool MainAsk(){
			this->finish = false;
			Request();
			while (true){

			}
		}
	private:
		string url;
		string document;
		bool finish;
		DWORD thread_id;
	    void Request(){
			//set the document acconding to the path
			document = "";
			try{
			}
			catch(exception e){

			}
		}
	};
}
