/**
*@file
*@brief sending and receiving message from client
*@author  ACM2012
*@date 2015/04/19
*@version 0.1
*@note
*As to send message
*the format is CMD + " " + PATH + " " + HOST
*As to receive message, the format is
*FILE TYPE + " " + FILE LEN + " " + FILE CONTENT
*/
#pragma once
#include "map"
#include "Tools.h"
#include "MyServeSocket.h"
/**@brief Byte*/
#define Byte (unsigned char) 
/**@brief Socket Number*/
#define SCOKETNUM 1024
/**@brief No Socket*/
#define NO_SOCKET 1
/**@brief Not Receive Data*/
#define NO_REV_DATA 2
/**@brief Port*/
#define PORT 6500
/**@brief Return the Message OK*/
#define MSG_OK 200
/**@brief Return the Message BAD_REQUEST*/
#define BAD_REQUEST 400
/**@brief Return the Message Not Found*/
#define NO_FOUND 404

using namespace Tools; ///<using the namespace Tools
typedef unsigned char Byte;

namespace NetWork{
	/**
	*@class AppLayerHttp NetWork
	*@brief How the Application Layer works
	*@author  ACM2012
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
		*@author  ACM2012
		*@param [in] <RevData> the data waiting to analysis
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
		*@author  ACM2012
		*@param [in] <CMD> the Request Methods of HTTP protocol
		*@param [in] <args> some arguments with the CMD
		*@return void
		*@note
		*map the CMD to the real function
		*@remarks
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
		/**@brief
		 *the Socket is the class we design, map it to the count
		 *map <ClientSocket *, int> myclient;
		 *the file in the "/" path
		 */
		int ErrorCode;
		/**@brief the Msg respond to client*/
		string RespondMsg;

		/**
		*@brief the function we choose to deal with the data request
		*@author  ACM2012
		*@param [in] <data> some information we want to get
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

		/**
		*@brief the function we choose to deal with the data request like GET
		*@author  ACM2012
		*@param [in] <data> some information we want to get
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
		/**
		*@brief POST methods
		*@author  ACM2012
		*@param [in] <data> some information we want to get
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
		/**
		*@brief PUT method
		*@author  ACM2012
		*@param [in] <data> some information we want to get
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

		/**
		*@brief DELETE some options of request methods
		*@author  ACM2012
		*@param [in] <data> some information we want to get
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

		/**
		*@brief some options of request methods
		*@author  ACM2012
		*@param [in] <data> some information we want to get
		*@return void
		*@note
		*关于服务器支持的请求方法信息的请求,Map it to 6
		*just tell the people things this serve support
		* 1 return OK
		* 2 return "GET HEAD POST PUT DELETE OPTIONS TRACE CONNECT"as Msg
		*/
		void OPTIONS(vector<string> data){

		}
		/**
		*@brief don't Know what's it meaning
		*@author  ACM2012
		*@param [in] <data> some information we want to get
		*@return void
		*@note
		*Web服务器反馈Http请求和其头标的请求,Map it to 7
		*/
		void TRACE(vector<string> data){
			//I don't Know what's it meaning?
		}

		/**
		*@brief NOT USE FUNCTION
		*@author  ACM2012
		*@param [in] <data> some information we want to get
		*@return void
		*@note
		*已文档化但当前未实现的一个方法,预留做隧道处理, Map it to 8
		*this function leaved to satifiy other function
		*and it doesn't has a function now
		*/
		void CONNECT(vector<string> data){

		}

		/**
		*@brief init the Http Object
		*@author  ACM2012
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

		/**
		*@brief launching a listen thread
		*@author  ACM2012
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

	/**
	*@class <MySocket>
	*@brief TCP和UDP的公共部分
	*@author  ACM2012
	*@note
	*/
	class MySocket{
	public:
		//NOT USE NOW
		/*
		bool data_alloc(int len, T Msg, int Word_size){
			this->data = (Byte *)malloc(sizeof(Byte)*len);
			int i;
			for (i = 0; i < len; i++){
				while (Word_size){
					Word_size -= 8;

				}
			}
		}*/

		/**
		*@author  ACM2012
		*@note
		*标准构造函数
		*@remarks
		*/
		MySocket(){
			message.head_len = 0;
			message.data_len = 0;
			message.head = NULL;
			message.data = NULL;
		}

		/**
		*@author  ACM2012
		*@param [in] <len> the length of data
		*@param [in] <Msg> the length of Message header
		*@note
		*if TCP & UDP to create a object, and they want to recv Msg, use it
		*also you can use this methon when sending a Msg
		*@remarks
		*/
		MySocket(int head_len, int data_len){
			message.head_len = head_len;
			message.data_len = data_len;
			message.head = (Byte *)malloc(sizeof(Byte) * message.head_len);
			message.data = (Byte *)malloc(sizeof(Byte) * message.data_len);
		}

		/**
		*@author  ACM2012
		*@param [in] <len> the length of data
		*@param [in] <Msg> the Message
		*@return return true if allocate memory to data successfully
		*@note
		*if get a string , I'd like to turn it to Byte
		*@remarks
		*/
		bool data_alloc(int len, string Msg){
			//if there are data exist in data, delete it
			if (message.data_len) delete message.data;
			//create it
			this->message.data = (Byte *)malloc(sizeof(Byte)*len);
			int i;
			for (i = 0; i < len; i++){
				this->message.data[i] = Msg[i];
			}
			return true;
		}

		/**
		*@author  ACM2012
		*@param [in] <len> the length of data
		*@param [in] <Msg> the address of Message
		*@return return true if allocate memory to data successfully
		*@note
		*if get a char array , just set it to Byte
		*@remarks
		*/
		bool data_alloc(int len, Byte *Msg){
			//if there are data exist in data, delete it
			if (message.data_len) delete message.data;
			//create it
			this->message.data = (Byte *)malloc(sizeof(Byte)*len);
			int i;
			for (i = 0; i < len; i++){
				this->message.data[i] = Msg[i];
			}
			return true;
		}
		/**
		*@author  ACM2012
		*@param [in] <head> the head of message
		*@return void
		*@note
		*if get a string , I'd like to turn it to Byte
		*@remarks
		*/
		void SetHead(string head){
			this->message.head = (Byte *)malloc(sizeof(Byte)*head.length());
			memcpy(this->message.head, head.c_str(), head.length());
			this->message.head_len = head.length();
		}
		/**
		*@author  ACM2012
		*@param [in] <head> the head of message
		*@param [in] <len> the length of message
		*@return void
		*@note
		*if get a char array , just set it to Byte
		*@remarks
		*/
		void SetHead(Byte *head, int len){
			this->message.head = head;
			this->message.head_len = len;
		}

		/**
		*@brief send the Msg to NET layer
		*@author  ACM2012
		*@return if(!nettran(message)) return false
		*@note
		* if the NET layer has the CHICK
		JUST CHICK IT
		if the len don't satisfy it, return false
		if the NET is busy or not preparing well, return false
		goto send
		NET nettran
		if(!nettran(message)) return false;
		*@remarks
		*/
		bool Sender(){
			//if the NET layer has the CHICK
			//JUST CHICK IT
			//if the len don't satisfy it, return false
			//if the NET is busy or not preparing well, return false 
			//goto send
			//NET nettran
			//if(!nettran(message)) return false;
			return true;
		}
		/**
		*@author  ACM2012
		*@return if(!netrevc(&message)) return false
		*@note
		*get the Msg from NET layer
		*@remarks
		*/
		bool Revcer(){
			//if(!netrevc(&message)) return false;
			//JUST FINISHED
			return true;
		}
		/**@brief the check function, return the answer */
		short CheckSum(){
			//steps 
		}
		/**@brief check the Msg is right or not*/
		bool CheckSum(short num){
			//steps
		}
	private:
		struct Msg{
		    Byte *data;///<the data
			int data_len;
		    Byte *head;///<brief the head before data
			int head_len;
		};
		Msg message;
	};

	class MyUDP :MySocket{

	};

}
