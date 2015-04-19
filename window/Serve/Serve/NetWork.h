#pragma once
#include "map"
#include "Tools.h"
#include "MyServeSocket.h"
#define SCOKETNUM 1024
//
#define NO_SOCKET 1
//
#define NO_REV_DATA 2
//
#define PORT 6500
//
#define MSG_OK 200
//
#define BAD_REQUEST 400
//
#define NO_FOUND 404
/*
Rev sample Msg Data
CMD + " " + PATH + " " + HOST
Send
FILE TYPE + " " + FILE LEN + " " + FILE
*/
using namespace Tools;
namespace NetWork{
	/*HOW IT WORKS
	*/
	class AppLayerHttp{
	public:
		string Log;
		AppLayerHttp(){
			Http_init();//init the map function
			//new the object, and there must be none connected
			Listen_launch();//launch a listen thread
		}
		~AppLayerHttp(){
			delete &Function;//remove the map
		}
		/*name: Getpath
		  function: analysis the path send by 
		*/
		/* divide the data recived
		using tools::Split
		*/
		void GetData(string RevData){
			//div by some char ' '
			//save them in data
			STR::Split(RevData, &data, ' ');
		}
		//map the CMD to the real function
		void DealWith(string CMD, vector<string> args){
			if (Function.find(CMD)==Function.end()){
				//DON'T HAVE THIS CMD
				//return BADxxx
				ErrorCode = BAD_REQUEST;
			}
			try{
				//注意1：这里必须使用this->，否则会有error C2171: “*”: 
				//“void (__thiscall CUtil::* )(int)”类型的操作数非法的错误
				//注意2：这里必须使用*解引用，才能实现函数调用，
				//否则会有error C2064: 项不会计算为接受 1 个参数的函数的错误
				(this->*Function[CMD])(args);
			}
			catch (exception e){
				Log = e.what();
				return ;
			}
		}
	private :
		/*the deal function struct*/
		typedef void(AppLayerHttp::*DealWithFunciton)(vector<string> data);
		/*the map struct*/
		map<string, DealWithFunciton> Function;
		/*the divided data*/
		vector<string> data;
		//
		bool Mark[SCOKETNUM];
		//turn 
		MyServeSocket *mysocket[SCOKETNUM];
		//
		/*the Socket is the class we design, map it to the count*/
		//map <ClientSocket *, int> myclient;
		//the file in the "/" path
		int ErrorCode;
		//the Msg respond to client
		string RespondMsg;
		/* the function we choose to deal with the data request*/
		/*检索URI中标识资源的一个简单请求, Map it to 1*/
		/*TOYOU : find GET message from net and copy them to here*/
		void GET(vector<string> data){
			//path >> data[1]
			//host >> data[2]
			/*
			1 find the file according to the path
			2 if not exist return NO_FOUND
			3 else use byte cache[1024] to divide the file
			4 read file into IO
			5 use a loop to do{
			    move parts of file into cache
				use the sender to send the cache out
				check the return value
			}
			*/
		}
		/*与GET方法相同，服务器只返回状态行和头标，
		并不返回请求文档, Map it to 2*/
		/*TOYOU : find HEAD message from net and copy them to here*/
		void HEAD(vector<string> data){
			//path >> data[1]
			//host >> data[2]
			/*
			1 find the file according to the path
			2 if not exist return (NO_FOUND,"NOT FOUND")
			given the Msg like (no of state, and text Msg)
			use sender to send it out
			}
			*/
		}
		/*服务器接受被写入客户端输出流中的数据的请求,
		Map it to 3*/
		/*TOYOU : find POST message from net and copy them to here*/
		void POST(vector<string> data){
			//path >> data[1]
			//host >> data[2]
			/*
			1 analysis the data from client
			2 save it the Socket conn to it
			return OK
			}
			*/
		}
		/*服务器保存请求数据作为指定URI新内容的请求,
		Map it to 4*/
		/*TOYOU : find PUT message from net and copy them to here*/
		void PUT(vector<string> data){
			//path >> data[1]
			//host >> data[2]
			/*
			1 find the file in path?
			2 no -> save it in this path ,return OK
			3 yes -> 
			  3.1 rewrite it in this path , return OK
			  3.2 return HAVE_EXISTED
			*/
		}
		/*服务器删除URI中命名的资源的请求, Map it to 5*/
		/*TOYOU : find DELETE message from net and copy them to here*/
		void DELETE(vector<string> data){
			//path >> data[1]
			//host >> data[2]
			/*
			1 find the file in path?
			2 yes -> delete it in this path ,return OK
			3 no ->
			3.1 return OK anyway
			3.2 return warnning "NOT_EXIST"
			*/
		}
		/*关于服务器支持的请求方法信息的请求,
		Map it to 6*/
		/*TOYOU : find OPTIONS message from net and copy them to here*/
		void OPTIONS(vector<string> data){
			/*just tell the people things this serve support
			1 return OK
			2 return "GET HEAD POST PUT DELETE OPTIONS TRACE CONNECT"
			  as Msg
			*/
		}
		/*Web服务器反馈Http请求和其头标的请求,
		Map it to 7*/
		/*TOYOU : find TRACE message from net and copy them to here*/
		void TRACE(vector<string> data){
			//I don't Know what's it meaning?
		}
		//NOT USE FUNCTION
		/*已文档化但当前未实现的一个方法，
		预留做隧道处理, Map it to 8*/
		void CONNECT(vector<string> data){
			//this function leaved to satifiy other function
			//and it doesn't has a function now
		}
		//LIKE COPY MOVE LINK UNLINK and so on
		/*
		  Name : Http_init
		  function: init the Http Object
		  Init the Map 
		  该映射不分辨大小写
		*/
		void Http_init(){
			/*检索URI中标识资源的一个简单请求, Map it to 1*/
			Function["GET"] = Function["get"] = &AppLayerHttp::GET;
			/*与GET方法相同，服务器只返回状态行和头标，
			  并不返回请求文档, Map it to 2*/
			Function["HEAD"] = Function["head"] = &AppLayerHttp::HEAD;
			/*服务器接受被写入客户端输出流中的数据的请求,
			  Map it to 3*/
			Function["POST"] = Function["post"] = &AppLayerHttp::POST;
			/*服务器保存请求数据作为指定URI新内容的请求,
			  Map it to 4*/
			Function["PUT"] = Function["put"] = &AppLayerHttp::PUT;
			/*服务器删除URI中命名的资源的请求, Map it to 5*/
			Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETE;
			/*关于服务器支持的请求方法信息的请求, 
			  Map it to 6*/
			Function["OPTIONS"] = Function["options"] = &AppLayerHttp::OPTIONS;
			/*Web服务器反馈Http请求和其头标的请求,
			  Map it to 7*/
			Function["TRACE"] = Function["trace"] = &AppLayerHttp::TRACE;
			/*已文档化但当前未实现的一个方法，
			  预留做隧道处理, Map it to 8*/
			Function["CONNECT"] = Function["connet"] = &AppLayerHttp::CONNECT;
		}
		
		/*name: Listen_launch
		  function: 

		*/

		bool Listen_launch(){
			//launch a listen thread
			//if failed return false;
			//else return true
			//new a socket to listen from the request from client
			/*STEP:
			1 FREE A CSocket
			2 USE THE No.0
			3 0 to do other things
			4 Open the listener
			return ture, else return false
			*/
		}


	};
}
