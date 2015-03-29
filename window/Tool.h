//tools for other to use
#include <cstring>
#include <thread>
#include <windows.h>
#include "stdafx.h"
#pragma once
using namespace std;
namespace Tools{
	//overload : it don't need to use _T anymore
    void AfxMessageBox(string Msg){
		AfxMessageBox(Msg);
	}
	//Dllusr
	class Dlluser{


	public :
		void Getpath(string path){
			this->path = path;
		}

	private:
		string path;
	};
	
	//get html 
	class Htmlget{
	public:
		void GetUrl(string path){
			this->path = path;
		}
		string GetText(){
			return this->document;
		}
		bool MainAsk(){
			document = "";
			//open a thread to Request
			hThread = CreateThread(NULL, 0, this->Request, 0, 0, &thread_id);
			if (hThread == NULL) {
				AfxMessageBox(" 系统创建线程失败，没有获取信息\n ");
				return false;//_T()
			}
			while (true){
				if (document != "") break;
			}
			//destory
			ExitThread(thread_id);
			return true;
		}
	private:
		string path;
		string document;
		DWORD thread_id;
		HANDLE hThread;
		void Request(){
			//set the document acconding to the path
		}
	};
}