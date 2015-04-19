/**@file
 *@brief sending and receiving message from client
 *@author xinYao,jinxiaFang
 *@date 2015/04/19
 *@version XXXXXX
 *@note
 *As to send message, the format is CMD + " " + PATH + " " + HOST
 *As to receive message, the format is FILE TYPE + " " + FILE LEN + " " + FILE CONTENT
 */

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

using namespace Tools;
namespace NetWork{
	/**
	 *@class AppLayerHttp NetWork
	 *@brief How the Application Layer works
	 *@author xinYao, jinxiaFang
	 *@note
	 *to achieve communication of the client and the server with HTTP protocol
	 */
	class AppLayerHttp{
	public:
		string Log;
		/**@brief How the Application Layer works*/
		AppLayerHttp(){
		    /**@brief init the map function*/
			Http_init();
			/**@brief launch a listen thread*/
			Listen_launch();
		}
		/**@brief remove the map*/
		~AppLayerHttp(){
			delete &Function;
		}

		/**
         *@brief analyse the data send by
         *@author
         *@param in RevData the data waiting to analysis
         *@exception XXXXXXX
         *@return void
         *@note
         *To achieve dividing the data recived using tools::Split
         *and save them in data
         *@remarks the format of data is splited by " "
		 */
		void GetData(string RevData){
			STR::Split(RevData, &data, ' ');
		}

		/**
         *@brief to handle the CMD
         *@author
         *@param in CMD the Request Methods of HTTP protocol
         *@param in args some arguments with the CMD
         *@exception XXXXXXX
         *@return void
         *@note
         *map the CMD to the real function
         *@remarks XXX
		 */
		void DealWith(string CMD, vector<string> args){
		    /**@brief DON'T HAVE THIS CMD return BADxxx */
			if (Function.find(CMD)==Function.end()){

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
		/**@brief the deal function struct*/
		typedef void(AppLayerHttp::*DealWithFunciton)(vector<string> data);
		/**@brief the map struct*/
		map<string, DealWithFunciton> Function;
		/**@brief the divided data*/
		vector<string> data;
		/**@brief  */
		bool Mark[SCOKETNUM];
		/**@brief turn */
		MyServeSocket *mysocket[SCOKETNUM];
		/**@note
		 *the Socket is the class we design, map it to the count
		 *map <ClientSocket *, int> myclient;
		 *the file in the "/" path
		 */
		int ErrorCode;
		/**@brief the Msg respond to client*/
		string RespondMsg;

		/**@brief the function we choose to deal with the data request
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *检索URI中标识资源的一个简单请求, Map it to 1
		 *path >> data[1]
         *host >> data[2]
         *1 find the file according to the path
         *2 if not exist return NO_FOUND
         *3 else use byte cache[1024] to divide the file
         *4 read file into IO
         *5 use a loop to do{
			    move parts of file into cache
				use the sender to send the cache out
				check the return value
			}
		 */
		void GET(vector<string> data){

		}

		/**@brief the function we choose to deal with the data request like GET
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *与GET方法相同，服务器只返回状态行和头标，
		 *并不返回请求文档, Map it to 2
		 *path >> data[1]
         *host >> data[2]
         *1 find the file according to the path
         *2 if not exist return (NO_FOUND,"NOT FOUND")
			given the Msg like (no of state, and text Msg)
			use sender to send it out
		 */
		void HEAD(vector<string> data){

		}
		/**@brief POST methods
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *服务器接受被写入客户端输出流中的数据的请求,Map it to 3
		 *path >> data[1]
         *host >> data[2]
         *1 analysis the data from client
         *2 save it the Socket conn to it
         *return OK
         */
		void POST(vector<string> data){

		}
		/**@brief PUT method
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *服务器保存请求数据作为指定URI新内容的请求,Map it to 4
		 *path >> data[1]
         *host >> data[2]
         *1 find the file in path?
         *2 no -> save it in this path ,return OK
         *3 yes ->
         * 3.1 rewrite it in this path , return OK
         * 3.2 return HAVE_EXISTED
         */
		void PUT(vector<string> data){

		}

		/**@brief DELETE some options of request methods
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *服务器删除URI中命名的资源的请求, Map it to 5
		 *path >> data[1]
		 *host >> data[2]
		 *1 find the file in path?
         *2 yes -> delete it in this path ,return OK
         *3 no ->
         * 3.1 return OK anyway
         * 3.2 return warnning "NOT_EXIST"
         */
		void DELETE(vector<string> data){

		}

		/**@brief some options of request methods
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *关于服务器支持的请求方法信息的请求,Map it to 6
		 *just tell the people things this serve support
		 * 1 return OK
		 * 2 return "GET HEAD POST PUT DELETE OPTIONS TRACE CONNECT"as Msg
		 */
		void OPTIONS(vector<string> data){

		}

		/**@brief don't Know what's it meaning
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *Web服务器反馈Http请求和其头标的请求,Map it to 7
		 */
		void TRACE(vector<string> data){
			//I don't Know what's it meaning?
		}

		/**@brief NOT USE FUNCTION
		 *@author
		 *@param in data some information we want to get
		 *@return void
		 *@note
		 *已文档化但当前未实现的一个方法,预留做隧道处理, Map it to 8
		 *this function leaved to satifiy other function
		 *and it doesn't has a function now
		 */
		void CONNECT(vector<string> data){

		}

		/**@brief init the Http Object
		 *@author
		 *@param
		 *@return void
		 *@note
		 *Init the Map
         *该映射不分辨大小写
		 */
		void Http_init(){
			/**@brief 检索URI中标识资源的一个简单请求, Map it to 1*/
			Function["GET"] = Function["get"] = &AppLayerHttp::GET;
			/**@brief 与GET方法相同，服务器只返回状态行和头标，
			  并不返回请求文档, Map it to 2*/
			Function["HEAD"] = Function["head"] = &AppLayerHttp::HEAD;
			/**@brief 服务器接受被写入客户端输出流中的数据的请求,
			  Map it to 3*/
			Function["POST"] = Function["post"] = &AppLayerHttp::POST;
			/**@brief 服务器保存请求数据作为指定URI新内容的请求,
			  Map it to 4*/
			Function["PUT"] = Function["put"] = &AppLayerHttp::PUT;
			/**@brief 服务器删除URI中命名的资源的请求, Map it to 5*/
			Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETE;
			/**@brief 关于服务器支持的请求方法信息的请求,
			  Map it to 6*/
			Function["OPTIONS"] = Function["options"] = &AppLayerHttp::OPTIONS;
			/**@brief Web服务器反馈Http请求和其头标的请求,
			  Map it to 7*/
			Function["TRACE"] = Function["trace"] = &AppLayerHttp::TRACE;
			/**@brief 已文档化但当前未实现的一个方法，
			  预留做隧道处理, Map it to 8*/
			Function["CONNECT"] = Function["connet"] = &AppLayerHttp::CONNECT;
		}

		/**@brief launching a listen thread
		 *@author
		 *@param
		 *@return void
		 *@note
		 *launch a listen thread
		 *if failed return false, else return true
         *new a socket to listen from the request from client
         *STEP:
		 * 1 FREE A CSocket
		 * 2 USE THE No.0
		 * 3 0 to do other things
		 * 4 Open the listener
		 * return ture, else return false
		 */

		bool Listen_launch(){

		}


	};
}
