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
#include<io.h>
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
/**@brief Return the Message Created*/
#define Created 201
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
				delete temp;
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
			RespondMsg = "PUT :";
			//string p = servepath + data[1];
			char path[1024];
			int retMsg;
			int i;
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (!fopen_s(&fp, path, "r"))
			{
				fclose(fp);
				DeleteFile(STR::String2LPCWSTR(data[1]));
				ErrorCode = MSG_OK;
				RespondMsg += "change the file existed in serve";
			}
			else
				ErrorCode = Created;
			fopen_s(&fp, path, "w");
			char *temp;
			if (data.size() < 3) data.push_back("");
			temp = (char *)malloc(sizeof(char)*(data[2].length() + 1));
			for (i = 0; i < data[2].length(); i++)
				temp[i] = data[2][i];
			temp[i] = 0;
			fprintf_s(fp, "%s\n",temp);
			free(temp);
			fclose(fp);
			return;
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
		
		void DELETEFILE(vector<string> data){
			//find file
			RespondMsg = "File"+ data[1] + " have been DELETE";
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
			fclose(fp);
			DeleteFile(STR::String2LPCWSTR(data[1]));
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
			//find files
			RespondMsg = "OPTIONSMSG : ";
			ErrorCode = MSG_OK;
			RespondMsg += "GET : ";
			RespondMsg += "Retrieve a simple request URL identifying the resources.";
			RespondMsg += "\r\n";

			RespondMsg += "HEAD : ";
			RespondMsg += "Same as the GET method, the server returns only the status line and head.";
			RespondMsg += "\r\n";

			RespondMsg += "POST : "; 
			RespondMsg += "The server accepts the request of the data that are written to the client output stream.";
			RespondMsg += "\r\n";

			RespondMsg += "PUT : ";
			RespondMsg += "The server save request data as the specified URL request of new content.";
			RespondMsg += "\r\n";

			RespondMsg += "DELETE : ";
			RespondMsg += "delete the file on the server according to URL";
			RespondMsg += "\r\n";

			RespondMsg += "OPTIONS : ";
			RespondMsg += "The request of the information about request methods of the server supports.";
			RespondMsg += "\r\n";

			RespondMsg += "CONNECT : "; 
			RespondMsg += "The method, which has documented but not implented currently, reserved for the tunnel processing.";
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
			RespondMsg = "The request from client has been received"; 
			//The Method has been documented but is not currently implented ,reserved for channel processing 
        
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
			Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETEFILE;
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

#define FTP_PORT 7600
#define root_power 0
#define user_power 3
#define Sign_SU 200
#define Sign_Fail 300
#define OP_OK 120
#define OP_Fail 302

	class AppLayerFtp{
		class people{
		public:
			string name;
			int p;
			string path;
		};
	public:
		AppLayerFtp(){
			/**@brief init the map function*/
			Ftp_init();
			/**@brief launch a listen thread*/
		}
		/**@brief remove the map*/
		~AppLayerFtp(){
			//delete &Function;
		}

		void GetData(string RevData){
			STR::Split(RevData, &data, ' ');
		}
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
			if (Function.find(CMD) == Function.end()){
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
				return;
			}
		}

	private:
		people *Sign_in;
		string Log;
		typedef void(AppLayerFtp::*DealWithFunciton)(vector<string> data);
		map<string, DealWithFunciton> Function;
		vector<string> data;
		int ErrorCode;
		string RespondMsg;
		//CMD ＋ user + pass
		void SIGNIN(vector<string> data){
			//char path[1024];
			Sign_in = new people();
			Sign_in->path = "ftp/" + data[1] + data[2];
			FILE *fp;
			if (!fopen_s(&fp, Sign_in->path.c_str(), "r")){
				
			}
			else{
				delete Sign_in;
				ErrorCode = Sign_Fail;
				return;
			}
		}
		void QUIT(vector<string> data){
			delete Sign_in;
			Sign_in = NULL;
			ErrorCode = OP_OK;
			return;
		}
		void CD(vector<string> data){
			struct _finddata_t files;
			int File_Handle;
			int i = 0;
			string *temp;
			data[1] = data[1] + "/*";

			File_Handle = _findfirst(data[1].c_str() , &files);
			if (File_Handle == -1)
			{
				ErrorCode = OP_Fail;
				return;
			}
			do
			{
				//printf("%s \n",files.name);
				temp=new string(files.name);
				RespondMsg = *temp + " ";
			} while (0 == _findnext(File_Handle, &files));
			_findclose(File_Handle);
			//printf("Find %d files\n",i);
			return;
		}
		void MKDIR(vector<string> data){
			
		}
		void DELETEFILE(vector<string> data){
			//find file
			RespondMsg = "File" + data[1] + " have been DELETE";
			//string p = servepath + data[1];
			char t[1024];
			char path[1024];
			int i;
			data[1] = Sign_in->path +"/"+ data[1];
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (fopen_s(&fp, path, "r")){
				ErrorCode = OP_Fail;
				return;
			}
			ErrorCode = OP_OK;
			fclose(fp);
			DeleteFile(STR::String2LPCWSTR(data[1]));
		}
		void UPLOAD(vector<string> data){
			//find files
			RespondMsg = "UPLOAD :";
			//string p = servepath + data[1];
			char path[1024];
			int retMsg;
			int i;
			data[2] = Sign_in->path +"/"+ data[2];
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (!fopen_s(&fp, path, "r"))
			{
				fclose(fp);
				DeleteFile(STR::String2LPCWSTR(data[1]));
				ErrorCode = OP_OK;
				RespondMsg += "change the file existed in serve";
			}
			else
				ErrorCode = OP_OK;
			fopen_s(&fp, path, "w");
			char *temp;
			if (data.size() < 3) data.push_back("");
			temp = (char *)malloc(sizeof(char)*(data[2].length() + 1));
			for (i = 0; i < data[2].length(); i++)
				temp[i] = data[2][i];
			temp[i] = 0;
			fprintf_s(fp, "%s\n", temp);
			free(temp);
			fclose(fp);
			return;

		}
		void DOWNLOAD(vector<string> data){
			RespondMsg = "";
			string *temp;
			//string p = servepath + data[1];
			char t[1024];
			char path[1024];
			int i;
			data[1] = Sign_in->path + "/" + data[1];
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			for (i = 0; i < data[1].length(); i++){
				path[i] = data[1][i];
			}
			path[i] = '\0';
			FILE *fp;
			if (fopen_s(&fp, path, "r")){
				ErrorCode = OP_Fail;
				return;
			}
			ErrorCode = OP_OK;
			while (fscanf_s(fp, "%s", t, 1024) != -1){
				temp = new string(t);
				RespondMsg += *temp;
				delete temp;
			}
			fclose(fp);
			
		}
		void ADDUSER(vector<string> data){
			RespondMsg = "The request from client has been received";
			//The Method has been documented but is not currently implented ,reserved for channel processing 

		}
		void DELUSER(vector<string> data){
			RespondMsg = "The request from client has been received";
			//The Method has been documented but is not currently implented ,reserved for channel processing 

		}
		void HELP(vector<string> data){
			RespondMsg = "HELPMSG : ";
			ErrorCode = OP_OK;
			RespondMsg += "SIGNIN : ";
			RespondMsg += "Log on to server.";
			RespondMsg += "\r\n";

			RespondMsg += "QUIT : ";
			RespondMsg += "Log out from server.";
			RespondMsg += "\r\n";

			RespondMsg += "CD : ";
			RespondMsg += "Open an existed folder.";
			RespondMsg += "\r\n";

			RespondMsg += "MKDIR : ";
			RespondMsg += "Creat a new folder.";
			RespondMsg += "\r\n";

			RespondMsg += "DELETEFILE : ";
			RespondMsg += "delete the file on the server ";
			RespondMsg += "\r\n";

			RespondMsg += "UPLOAD : ";
			RespondMsg += "Upload the file to the server.";
			RespondMsg += "\r\n";

			RespondMsg += "DOWNLOAD : ";
			RespondMsg += "Download the requested file from server.";
			RespondMsg += "\r\n";

			RespondMsg += "ADDUSER : ";
			RespondMsg += "Add a new user.";
			RespondMsg += "\r\n";

			RespondMsg += "DELUSER : ";
			RespondMsg += "Delete an existed user.";
			RespondMsg += "\r\n";

			RespondMsg += "HELP : ";
			RespondMsg += "The request of the information about request methods of the server supports.";
			RespondMsg += "\r\n";
			//The Method has been documented but is not currently implented ,reserved for channel processing 

		}
		
		void Ftp_init(){
			Function["SIGNIN"] = Function["signin"] = &AppLayerFtp::SIGNIN;
			Function["QUIT"] = Function["quit"] = &AppLayerFtp::QUIT;
			Function["CD"] = Function["cd"] = &AppLayerFtp::CD;	
			Function["MKDIR"] = Function["mkdir"] = &AppLayerFtp::MKDIR;
			Function["DELETE"] = Function["delete"] = &AppLayerFtp::DELETEFILE;	
			Function["UPLOAD"] = Function["upload"] = &AppLayerFtp::UPLOAD;
			Function["DOWNLOAD"] = Function["download"] = &AppLayerFtp::DOWNLOAD;
			Function["ADDUSER"] = Function["adduser"] = &AppLayerFtp::ADDUSER;
			Function["DELUSER"] = Function["deluser"] = &AppLayerFtp::DELUSER;
			Function["HELP"] = Function["help"] = &AppLayerFtp::HELP;
		}
	};
}
