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
#include <strstream>
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

#define SOCKET_ERROR -1
using namespace Tools; ///<using the namespace Tools
typedef unsigned char Byte;
/*
struct sockaddr_in
{
	short sin_family;//Addressfamily一般来说AF_INET（地址族）PF_INET（协议族）
	unsigned short sin_port;//Portnumber(必须要采用网络数据格式,普通数字可以用htons()函数转换成网络数据格式的数字)
	struct in_addr sin_addr;//Internetaddress
	unsigned char sin_zero[8];//Samesizeasstructsockaddr没有实际意义,只是为了　跟SOCKADDR结构在内存中对齐
};*/

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
			//delete &Function;
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
		
		string GetCMD(){
			return data[0];
		}
		int GetCode(){
			return ErrorCode;
		}
		string GetResMsg(){
			return RespondMsg;
		}
		//, vector<string> args
		void DealWith(string CMD){
		    /**@brief DON'T HAVE THIS CMD return BADxxx */
			if (Function.find(CMD)==Function.end()){
				ErrorCode = BAD_REQUEST;
				return;
			}
			try{
				//注意1：这里必须使用this->，否则会有error C2171: “*”:
				//“void (__thiscall CUtil::* )(int)”类型的操作数非法的错误
				//注意2：这里必须使用*解引用，才能实现函数调用，
				//否则会有error C2064: 项不会计算为接受 1 个参数的函数的错误
				(this->*Function[CMD])(data);//args
				
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
		*检索URL中标识资源的一个简单请求, Map it to 1
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
			//find file
			RespondMsg = "";
			string *temp;
			//string p = servepath + data[1];
			char t[1024];
			char path[1024];
			int i;
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (fopen_s(&fp, path, "r")){
				ErrorCode = NO_FOUND;
				return;
			}
			ErrorCode = MSG_OK;
			while (fscanf_s(fp, "%s", t, 1024)!= -1){
				temp = new string(t);
				RespondMsg += *temp;
			}
			fclose(fp);
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
			//find file
			RespondMsg = "HEADMSG :";
			string temp;
			//string p = servepath + data[1];
			char t[1024];
			char path[1024];
			int i;
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (fopen_s(&fp, path, "r")){
				ErrorCode = NO_FOUND;
				return;
			}
			ErrorCode = MSG_OK;
			strstream ss;
			ss << ErrorCode;
			ss >> temp;//Errorcode
			RespondMsg += temp;
			RespondMsg += "\r\n";
			RespondMsg += ("path in serve: " + data[1]);
			fclose(fp);
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
		/*
		void DELETE(vector<string> data){

		}*/

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
			//find files
			RespondMsg = "OPTIONSMSG : \r\n";
		//	string temp;
			//if (fopen_s(&fp, path, "r")){
			//	ErroeCode = ERROR;//return false message
			//	return;
			//}
			ErrorCode = MSG_OK;
			RespondMsg += "GET : ";
			RespondMsg += "Retrieve a simple request URL identifying the resources.";
			RespondMsg += "\r\n";

			RespondMsg += "HEAD : ";
			RespondMsg += "The same as the GET method, the server returns only the status line and head.";
			RespondMsg += "\r\n";

			RespondMsg += "POST : "; 
			RespondMsg += "The server accepts the request of the data that are written to the client output stream.";
			RespondMsg += "\r\n";

			RespondMsg += "PUT : ";
			RespondMsg += "The server save request data as the specified URL request of new content.";
			RespondMsg += "\r\n";

			RespondMsg += "OPTIONS : ";
			RespondMsg += "The request of the information about request methods of the server supports.";
			RespondMsg += "\r\n";

			RespondMsg += "CONNECT : "; 
			RespondMsg += "The method, which has documented but unrealized currently, reserved for the tunnel processing.";
			RespondMsg += "\r\n";
		}
		/**
		*@brief don't Know what's it meaning
		*@author  ACM2012
		*@param [in] <data> some information we want to get
		*@return void
		*@note
		*Web服务器反馈Http请求和其头标的请求,Map it to 7
		*/
		/*
		void TRACE(vector<string> data){
			//I don't Know what's it meaning?
		}*/

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
			//Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETE;
			/**@brief 关于服务器支持的请求方法信息的请求,
			  Map it to 6*/
			Function["OPTIONS"] = Function["options"] = &AppLayerHttp::OPTIONS;
			/**@brief Web服务器反馈Http请求和其头标的请求,
			  Map it to 7*/
			//Function["TRACE"] = Function["trace"] = &AppLayerHttp::TRACE;
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
			return true;
		}


	};

	/**
	*@class <MySocket>
	*@brief TCP和UDP的公共部分，支持可靠传输和不可靠传输的实现
	*@author  ACM2012
	*@note
	*Msg -> 报文
	*  head -> 报文头
	*  data -> 数据区
	*  IP -> 应用层的HOST
	*  MORE
	*/
	/** @file
	* @brief TCP UDP方法集
	* @author ACM2012
	* @date 2015/04/19
	* @version 0.1
	* @note
	* 用类似CSocket的方法定义TCP连接，但是实现时分别实现TCP，UDP的功能
	* 首先解决比较难的TCP连接，待TCP的功能实现后再来补全UDP的部分
	* 具体实现部分包括报文头的构造，连接的实现，可靠传输机制，流量控制等
	* 具体定义在后面详细说明
	* 该文件中的UDP类尚未定义,待实现TCP后，再来补全相应部分.
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
		*@return true
		*@note
		*if get a string , I'd like to turn it to Byte
		*@remarks
		*/
		bool SetHead(string head){
			this->message.head = (Byte *)malloc(sizeof(Byte)*head.length());
			memcpy(this->message.head, head.c_str(), head.length());
			this->message.head_len = head.length();
			return true;
		}
		/**
		*@author  ACM2012
		*@param [in] <head> the head of message
		*@param [in] <len> the length of message
		*@return true
		*@note
		*if get a char array , just set it to Byte
		*@remarks
		*/
		bool SetHead(Byte *head, int len){
			this->message.head = head;
			this->message.head_len = len;
			return true;
		}
		/**
		*@author  ACM2012
		*@param [in] <a> the head of message
		*@param [in] <b> the length of message
		*@param [in] <c> the head of message
		*@param [in] <d> the length of message
		*@return void
		*@note
		*
		*@remarks
		*/
		void SetIP(Byte a, Byte b, Byte c, Byte d){
			IP[0] = a;
			IP[1] = b;
			IP[2] = c;
			IP[3] = d;
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
		if(!nettran(message, IP)) return false;
		*@remarks
		*/
		bool Sender(){
			//if the NET layer has the CHICK
			//JUST CHICK IT
			//if the len don't satisfy it, return false
			//if the NET is busy or not preparing well, return false 
			//goto send
			//NET nettran
			//if(!nettran(message, IP)) return false;
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
		/*
		void ZeroMemory(Byte *recBuf){
			memset(recBuf, 0, sizeof(recBuf));
		}*/

		void OnReceive(int nErrorCode){

		}
	private:
		struct Msg{
		    Byte *data;///<the data
			int data_len;
		    Byte *head;///<brief the head before data
			int head_len;
		};
		Msg message;
		Byte IP[4];///<initial IP array
	};

	class MyUDP :MySocket{
		/**@brief 构造函数*/
		MyUDP();
		/**@brief 析构函数*/
		virtual ~MyUDP();

	protected:
		typedef Byte SOCKADDR;
		typedef Byte SOCKADDR_IN;

		SOCKADDR_IN m_clientAddr;
		
		int ReceiveFrom(Byte *Buf, int len, SOCKADDR* client_addr, int *addr_len, int mode){

		}
		/**@brief 重写OnReceive函数*/
		virtual void OnReceive(int nErrorCode)
		{
			Byte recBuf[1024];
			/**@brief Socket地址长度，属性继承自CAsyncSocket*/
			int len = sizeof(SOCKADDR_IN);
			/**@brief 清空buffer*/
			//ZeroMemory(recBuf);
			/**@brief 调用ReceiveFrom函数，接收数据*/
			int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_clientAddr, &len, 0);
			
			/**@brief 判断接收数据的情况*/
			if (recBytes == 0)
			{
				/**@brief 提示UDP未收到信息*/
			}
			else if (recBytes == SOCKET_ERROR)
			{
				/**@brief 提示接受错误*/
			}
			else
			{
				/**@brief 收到数据，将数据给出*/
			}

			MySocket::OnReceive(nErrorCode);
		}
		/**
		*@brief creat the message from received data
		*@author  ACM2012
		*@return if(data_alloc && sethead) true
		*@note
		*@remarks
		*/
		bool MsgMaker(){
			string Msg;
			int len;
			bool temp;
			//get the Msg and the length of Msg from APP
			temp = data_alloc(len, Msg);
			//get the IP from App
			//GetIP();
			//the port of serve is defined by 6500
			Byte  in_Port[2];
			//get Port from App
			//GetPort()
			Byte  out_Port[2];
			//transform message.data_len to Byte[2]
			Byte length[2];
			//CheckSum from message.data
			//transform the result of checksum to Byte[16];
			Byte check[2];
			CheckSum();
		    //从head指针开始，头2位放源端口号，3-4位放目的端口号，5-6位放收到的数据的长度，7-8位放检验和
			Byte *head = (Byte *)malloc(sizeof(Byte) * 8);
			//Byte *head = in_Port;
			//*(head + 2) = *out_Port;
			//*(head + 2) = *length;
			//*(head + 2) = *check;
			memcpy(head, in_Port, 2);
			memcpy(head + 2, out_Port, 2);
			memcpy(head + 4, length, 2);
			memcpy(head + 6, check, 2);
			//creat the head of the message
			if (temp && SetHead(head, sizeof(head)))
				return true;
			free(head);
		}
		/**
		*@brief 用来将上面生成的message发送出去
		*@author  ACM2012
		*@return 发送成功返回true，发送失败返回false
		*@note
		*@remarks
		*/
		bool UDP_Send(){
			//调用基类函数sender将上述生成的报文发出去
			if (Sender())
				return true;
			else return false;
		}
	};

	/**
	* @class
	* @brief 该类封装TCP协议方法.
	* @author ACM2012
	* @note
	* 此类包含该类的构造函数和析构函数，将实现TCP协议.
	* 构造函数MyTCP应传入对象进程的地址及端口号
	* 析构函数根据需要设置
	* 连接函数TCP_Conn 应当完成TCP协议的三次握手
	* 被连接函数TCP_Accept应当能够正确的和请求连接的进程进行通讯
	* 监视函数TCP_Listen应当监视外部请求，当收到请求时再调用其它成员来处理
	* 设置函数TCP_SetOpt应当设置TCP中各项参数，如报文头中部分参数以及拥塞控制相关参数
	* 不直接修改对应变量主要是出于安全考虑-------->>>>>初步实现可不使用此函数来控制
	* 发送函数Simple_Send实现简单的发送一段报文的功能，具体的可靠协议在TCP_Send中实现
	* 发送函数TCP_Send实现发送功能，同时实现中应该考虑到可靠传输的各项机制，如确认重传等
	* 接收函数TCP_Receive实现接收数据的功能，注意这个函数可以设置阻塞的功能，
	* 同时依然需要考虑可靠传输的问题
	* 解除连接TCP_DisConn 完成解除连接的功能
	* 类定义并不完整，只给出整体思路及框架
	*/

	class MyTCP :MySocket
	{
		/** @brief 构造函数*/
		MyTCP();
		/** @brief 析构函数*/
		~MyTCP();
	public:
		/** @brief 连接函数,
		* @note
		* 对于一个连接，应当返回连接成功与否相关信息
		* 此处应该完成三次握手的可靠方案
		* 包括SYN,ACK等标志位的设置
		*/
		bool TCP_Conn();

		/** @brief 被连接函数,
		* @note
		* 应当能够正确的和请求连接的进程进行连接
		*/
		void TCP_Accept();

		/** @brief 监听函数,
		* @note
		* 监视外部请求，当收到请求时再调用其它成员来处理
		*/
		void TCP_Listen();

		/** @brief 发送函数,
		* @note
		* 实现发送功能，同时实现中应该考虑到可靠传输的各项机制
		* 注意重传的参数，如定时等不应使用局部变量
		* 可靠传输包括：超时重传，三次冗余ACK重传
		* 重传使用回退N步方法
		*/
		void TCP_Send();

		/** @brief 接收函数,
		* @note
		* 实现接收数据的功能
		* 注意可靠机制下的确认报文发送
		* 需要解决处理丢失的方法，包括累积确认等
		*/
		void TCP_Receive();

		/** @brief 解除连接函数,
		* @note
		* 完成解除连接的功能
		*/
		void TCP_DisConn();
	private:
		/** @brief 报文头结构
		* @note
		*其中选项部分根据需要额外添加
		*/
		struct TCP_Head
		{
			unsigned short SourcePort;
			unsigned short DesPort;
			unsigned int No;
			unsigned int AckNo;
			short Len_Flag;
			unsigned short Receive_Window;
			unsigned short CheckSum;
			unsigned short Urgent_Pt;
		};
		/** @brief 简单发送
		* @note
		* 简单发送报文，用户不应该能使用这个函数，因为他并没有可靠机制
		*/
		void Simple_Send();
		/** @brief 简单接收
		* @note
		* 简单接收报文，用户不应该能使用这个函数，因为他并没有可靠机制
		*/
		void Simple_Receive();

		/** @brief 定义结构体，包含用于拥塞控制的参数
		* @note
		* SS:慢启动状态
		* CA：拥塞避免状态
		* RTT：一个往返时延
		* MSS：发送方与接收方协商最大报文段长度
		* CongWin：拥塞窗口
		* Threshold：阈值
		* redundancy_ACK：冗余ACK个数
		* state：当前状态
		* Time_out：超时
		*/
		struct CC
		{
			const Byte SS = 1;
			const Byte CA = 2;
			int RTT;
			int MSS;
			int CongWin;
			int Threshold;
			Byte redundancy_ACK;
			Byte state;
			bool Time_out;
		};

		/** @brief 拥塞控制函数,
		* @note
		* (1)收到未确认数据的ACK，
		* 若处于慢启动状态：每过一个RTT，拥塞窗口翻倍，若拥塞窗口超过阈值，状态改变为"拥塞避免"
		* 若处于拥塞避免状态：每过一个RTT，拥塞窗口增加一个MSS
		* (2)发生丢包，
		* 若由3个冗余ACK引发：拥塞窗口和阈值都设为CongWin的一半，设置状态为“拥塞避免”
		* 若由超时引发：阈值设为拥塞窗口的一半，拥塞窗口设置为1个MSS，设置状态为“慢启动”
		*/
		/*
		void Congestion_Control()
		{
			if (redundancy_ACK >= 3)
			{
				Threshold = CongWin / 2;
				CongWin = Threshold;
				state = CA;
			}
			else if (Time_out)
			{
				Threshold = CongWin / 2;
				CongWin = MSS;
				state = SS;
			}
			else
			{
				if (state == SS)
				{
					CongWin += MSS;
					if (CongWin > Threshold)
						state = CA;
				}
				else
				{
					CongWin += MSS * MSS / CongWin;
				}
			}
		}*/

	};
}
