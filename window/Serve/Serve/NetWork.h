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
#include "windows.h"
#include "HttpServeSocket.h"
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
	short sin_family;//Addressfamilyһ����˵AF_INET����ַ�壩PF_INET��Э���壩
	unsigned short sin_port;//Portnumber(����Ҫ�����������ݸ�ʽ,��ͨ���ֿ�����htons()����ת�����������ݸ�ʽ������)
	struct in_addr sin_addr;//Internetaddress
	unsigned char sin_zero[8];//Samesizeasstructsockaddrû��ʵ������,ֻ��Ϊ�ˡ���SOCKADDR�ṹ���ڴ��ж���
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
			if (Function.find(CMD)==Function.end( )){
				ErrorCode = BAD_REQUEST;
				RespondMsg += "The request is not supported.";
				RespondMsg += "\r\n";
				return;
			}
			try{
				/**@brief ע��1���������ʹ��this->���������error C2171: ��*��:*/
				/**@brief��void (__thiscall CUtil::* )(int)�����͵Ĳ������Ƿ��Ĵ��� */
				/** @brief ע��2���������ʹ��*�����ã�����ʵ�ֺ������ã�*/
				/**@brief �������error C2064: ������Ϊ���� 1 �������ĺ����Ĵ���*/
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
		 *map <HttpHttpClientSocket *, int> myclient;
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
		*����URL�б�ʶ��Դ��һ��������, Map it to 1
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
				RespondMsg += "The page does not exist.";
				RespondMsg += "\r\n";
				return;
			}
			ErrorCode = MSG_OK;
			RespondMsg += ".";
			RespondMsg += "\r\n";
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
		*��GET������ͬ��������ֻ����״̬�к�ͷ�꣬
		*�������������ĵ�, Map it to 2
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
				RespondMsg += "The page does not exist.";
				RespondMsg += "\r\n";
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
		*���������ܱ�д��ͻ���������е����ݵ�����,Map it to 3
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
		*��������������������Ϊָ��URI�����ݵ�����,Map it to 4
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
			RespondMsg += "The page is created.";
			RespondMsg += "\r\n";
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
		*������ɾ��URI����������Դ������, Map it to 5
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
				RespondMsg += "The file does not exist.";
				RespondMsg += "\r\n";
				return;
			}
			ErrorCode = MSG_OK;
			RespondMsg += "Successfully delete.";
			RespondMsg += "\r\n";
			fclose(fp);
			DeleteFile(STR::String2LPCWSTR(data[1]));
		}

		/**
		*@brief some options of request methods
		*@author  ACM2012
		*@param [in] <data> some information we want to get
		*@return void
		*@note
		*���ڷ�����֧�ֵ����󷽷���Ϣ������,Map it to 6
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
		*Web����������Http�������ͷ�������,Map it to 7
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
		*���ĵ�������ǰδʵ�ֵ�һ������,Ԥ�����������, Map it to 8
		*this function leaved to satifiy other function
		*and it doesn't has a function now
		*/
		void CONNECT(vector<string> data){ 
			RespondMsg = "The request from client has been received"; 
			//The Method has been documented but is not currently implented ,reserved for channel processing 
			ErrorCode = MSG_OK;
		}

		/**
		*@brief init the Http Object
		*@author  ACM2012
		*@return void
		*@note
		*Init the Map
		*��ӳ�䲻�ֱ��Сд
		*/
		void Http_init(){
			/**@brief ����URI�б�ʶ��Դ��һ��������, Map it to 1*/
			Function["GET"] = Function["get"] = &AppLayerHttp::GET;
			/**@brief ��GET������ͬ��������ֻ����״̬�к�ͷ�꣬
			  �������������ĵ�, Map it to 2*/
			Function["HEAD"] = Function["head"] = &AppLayerHttp::HEAD;
			/**@brief ���������ܱ�д��ͻ���������е����ݵ�����,
			  Map it to 3*/
			Function["POST"] = Function["post"] = &AppLayerHttp::POST;
			/**@brief ��������������������Ϊָ��URI�����ݵ�����,
			  Map it to 4*/
			Function["PUT"] = Function["put"] = &AppLayerHttp::PUT;
			/**@brief ������ɾ��URI����������Դ������, Map it to 5*/
			Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETEFILE;
			/**@brief ���ڷ�����֧�ֵ����󷽷���Ϣ������,
			  Map it to 6*/
			Function["OPTIONS"] = Function["options"] = &AppLayerHttp::OPTIONS;
			/**@brief Web����������Http�������ͷ�������,
			  Map it to 7*/
			//Function["TRACE"] = Function["trace"] = &AppLayerHttp::TRACE;
			/**@brief ���ĵ�������ǰδʵ�ֵ�һ��������
			  Ԥ�����������, Map it to 8*/
			Function["CONNECT"] = Function["connect"] = &AppLayerHttp::CONNECT;
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
#define QUIT_EXIT 303
#define Rev_OK 320

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
				//ע��1���������ʹ��this->���������error C2171: ��*��:
				//��void (__thiscall CUtil::* )(int)�����͵Ĳ������Ƿ��Ĵ���
				//ע��2���������ʹ��*�����ã�����ʵ�ֺ������ã�
				//�������error C2064: ������Ϊ���� 1 �������ĺ����Ĵ���
				RespondMsg = "";
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
		//CMD �� user + pass
		void SIGNIN(vector<string> data){//char path[1024];
			if (Sign_in != NULL) delete Sign_in;//over it
			Sign_in = new people();
			Sign_in->name = data[1];
			Sign_in->path = "ftp/" + data[1] + '_' + data[2];
			if (!_access(Sign_in->path.c_str(),0)){
				FILE *fp;
				string path = Sign_in->path + '/' + data[1] + ".config";
				if (!fopen_s(&fp, path.c_str(), "r")){
					fscanf_s(fp, "%d", &Sign_in->p);
				}
				RespondMsg = data[1] + " has logged in\r\n";
				ErrorCode = Sign_SU;
				if(Sign_in->name=="root") RespondMsg += "Superuser logs on.";
				RespondMsg += "\r\n";
				struct _finddata_t files;
				int File_Handle;
				string *temp;
				string pathfind = Sign_in->path + "/*";
				File_Handle = _findfirst(pathfind.c_str(), &files);
				if (File_Handle == -1){
					return;
				}
				do{
					temp = new string(files.name);
					RespondMsg += (*temp + " ");
				} while (0 == _findnext(File_Handle, &files));
				_findclose(File_Handle);
				return;
			}
			else{
				delete Sign_in;
				ErrorCode = Sign_Fail;
				RespondMsg += "Fail to log on to server.";
				RespondMsg += "\r\n";
				return;
			}
		}
		void QUIT(vector<string> data){
			delete Sign_in;
			Sign_in = NULL;
			ErrorCode = QUIT_EXIT;
			RespondMsg = "Sign Out!";
			return;
		}
		void CD(vector<string> data){
			struct _finddata_t files;
			int File_Handle;
			int i = 0;
			string *temp;
			Sign_in->path = Sign_in->path + "/" + data[1];
			data[1] = Sign_in->path+ "/*";
			File_Handle = _findfirst(data[1].c_str() , &files);
			if (File_Handle == -1){
				ErrorCode = OP_Fail;
				RespondMsg += "Fail to open folder.";
				RespondMsg += "\r\n";
				return;
			}
			RespondMsg += "Folder list.";
			RespondMsg += "\r\n";
			do{
				temp=new string(files.name);
				RespondMsg += (*temp + " ");
			} while (0 == _findnext(File_Handle, &files));
			_findclose(File_Handle);
			return;
		}
		void MKDIR(vector<string> data){
				//char filename[100], filename2[105];
			 struct _finddata_t files;
			 int File_Handle;
			 int i = 0;
			 string *temp;
			 string filename = Sign_in->path + "/" + data[1];
		     if (_access(filename.c_str(), 0)){//�ж�Ŀ¼�Ƿ����
				//filename = "md  "+ filename;
				//system(filename.c_str());
				CreateDirectory(STR::S2CS(filename), NULL);
				RespondMsg += "Creat success";
			}
			else{
				RespondMsg += "The fold alreay exists";
				RespondMsg += "/r/n";
			}
			data[1] = Sign_in->path + "/*";
			File_Handle = _findfirst(data[1].c_str(), &files);
			if (File_Handle == -1){
				ErrorCode = OP_Fail;
				RespondMsg += "Fail to creat folder.";
				RespondMsg += "\r\n";
				return;
			}
			do{
				temp = new string(files.name);
				RespondMsg += (*temp + " ");
			} while (0 == _findnext(File_Handle, &files));
			_findclose(File_Handle);
			return;
		}
		void DELETEFILE(vector<string> data){
			//find file
			RespondMsg += "File" + data[1] + " have been DELETE";
			RespondMsg += "\r\n";
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
				RespondMsg += "Fail to delete file.";
				RespondMsg += "\r\n";
				return;
			}
			ErrorCode = OP_OK;
			RespondMsg += "Successfully delete.";
			RespondMsg += "\r\n";
			fclose(fp);
			DeleteFile(STR::String2LPCWSTR(data[1]));
		}
		void UPLOAD(vector<string> data){
			//find files
			RespondMsg += "UPLOAD :";
			RespondMsg += "\r\n";
			//string p = servepath + data[1];
			char path[1024];
			int retMsg;
			int i;
			FILE *fp;
			data[1] = Sign_in->path +"/"+ data[1];
			ErrorCode = OP_OK;
			RespondMsg += "Successfully upload.";
			RespondMsg += "\r\n";
			//memcpy(path, data[1].c_str(), data.size());
			//path[data.size()] = '\0';
			fopen_s(&fp, data[1].c_str(), "w");
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
				RespondMsg += "Fail to download.";
				RespondMsg += "\r\n";
				return;
			}
			ErrorCode = Rev_OK;
			RespondMsg += "Successfully download.";
			RespondMsg += "\r\n";
			while (fscanf_s(fp, "%s", t, 1024) != -1){
				temp = new string(t);
				RespondMsg += *temp;
				delete temp;
			}
			fclose(fp);
		}
		void ADDUSER(vector<string> data){
			struct _finddata_t files;
			FILE* fp;
			int File_Handle;
			int i = 0;
			string *temp;
			string filename = "ftp/" + data[1] + '_' + data[2];
			if (Sign_in->p == root_power)
			{
				if (_access(filename.c_str(), 0))//�ж�Ŀ¼�Ƿ����
				{
					//filename = "md  " + filename;
					//system(filename.c_str());
					CreateDirectory(STR::S2CS(filename), NULL);
					RespondMsg += "Creat success\r\n";
					filename += "/";
					filename += (data[1] + ".config");
					fopen_s(&fp, filename.c_str(), "w");
					fprintf_s(fp, "%d\n", 3);
					fclose(fp);
				}
				else{
					RespondMsg += "The User alreay exists/r/n";
				}
				ErrorCode = OP_OK;
				RespondMsg += "Successfully add user.";
				RespondMsg += "\r\n";
			}
			else {
				ErrorCode = OP_Fail;
				RespondMsg += "User has no access to root";
				RespondMsg += "/r/n";
			}
		}
		void DELUSER(vector<string> data){
			int status;
			string filename;
			if (Sign_in->p == root_power)
			{
				filename = "ftp/" + data[1] + '_' + data[2];
				ErrorCode = OP_OK;
				status = _access(filename.c_str(), 0);
				if (status == 0){
					//op = "rmdir  " + Sign_in->path;
					//system(op.c_str());
					MYFILE::myDeleteDirectory(STR::S2CS(filename));
					RemoveDirectory(STR::S2CS(filename));
					RespondMsg += "User has been deleted\r\n";
				}
				else{
					RespondMsg += "No such User\r\n";
				}
			}
			else {
				ErrorCode = OP_Fail;
				RespondMsg += "User has no access to root";
				RespondMsg += "\r\n";
			}
		}
	 	void HELP(vector<string> data){
			RespondMsg = "HELPMSG : \r\n";
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

#ifndef SMTP
#define SMTP_PORT 8000
#define LOGIN 235
#define OK 250
#define GO 354
#define SMTP_QUIT 221
#define SYNTAX_ERROR 500
#define OPERATION_ERROR 550
#define RECEIVE_CMD 0
#define RECEIVE_MAIL 1
#endif
	class SMTP
	{
	public:
		SMTP()
		{
			memset(&M_re, 0, sizeof(M_re));
			Sign_in = 0;
			state = RECEIVE_CMD;
			ErrorCode = 0;
			RespondMsg = "";
		}
		~SMTP()
		{}
		int GetCode()
		{
			return ErrorCode;
		}
		string GetMsg()
		{
			return RespondMsg;
		}
		void CMD_Dispatch(string MSG)
		{
			this->MSG = MSG;
			if (state == RECEIVE_CMD)
			{
				STR::Split(MSG, &Split_msg, ' ');
				Split_msg[0] = STR::lower(Split_msg[0]);
				if (Split_msg[0] == "auth")
					auth();
				else if (Split_msg[0] == "noop")
					noop();
				else if (Split_msg[0] == "help")
					help();
				else if (Split_msg[0] == "data")
					data();
				else if (Split_msg[0] == "mail")
					from();
				else if (Split_msg[0] == "rcpt")
					to();
				else if (Split_msg[0] == "quit")
					quit();
				else if (Split_msg[0] == "rset")
					rset();
				else
				{
					//δ֪����
					ErrorCode = SYNTAX_ERROR;
					RespondMsg = "Unknown Command!\r\n";
				}
			}
			else
			if (state == RECEIVE_MAIL)
			{
				data();
			}
			//δ֪״̬
		}
	private:
		string RespondMsg;
		int ErrorCode;
		string MSG;
		vector<string> Split_msg;
		int state;
		int Sign_in;
		struct Mail
		{
			string to;
			string from;
			string content;
		}M_re;
	private:
		void rset()
		{
			memset(&M_re, 0, sizeof(M_re));
			ErrorCode = OK;
			RespondMsg = "Clear the data\r\n";
		}
		void auth()
		{
			if (Sign_in)
			{
				ErrorCode = OPERATION_ERROR;
				RespondMsg = "Already Login in!\r\n";
			}
			else
			if (Split_msg[1] == "login" || Split_msg[1] == "LOGIN")
			{
				ErrorCode = LOGIN;
				RespondMsg = "OK Authenticated!\r\n";
				Sign_in = 1;
			}
			else
			{
				ErrorCode = SYNTAX_ERROR;
				RespondMsg = "Unknown Command!\r\n";
			}
			return;
		}
		void noop()
		{
			ErrorCode = OK;
			RespondMsg = "OK!\r\n";
			return;
		}
		void help()
		{
			ErrorCode = OK;
			RespondMsg = "Supported Command:\r\n";
			RespondMsg += "\tauth login<CRLF>\r\n";
			RespondMsg += "\tMAIL FROM: xxx@xxx<CRLF>\r\n";
			RespondMsg += "\tRCPT TO: xxx@xxx<CRLF>\r\n";
			RespondMsg += "\tnoop<CRLF>\r\n";
			RespondMsg += "\thelp<CRLF>\r\n";
			RespondMsg += "\tdata<CRLF>(Mail)<CRLF>.<CRLF>\r\n";
			RespondMsg += "\trset<CRLF>\r\n";
//			RespondMsg += "\thelo Domain<CRLF>\r\n";
			RespondMsg += "\tquit<CRLF>\r\n";
			return;
		}
		void data()
		{
			if (Sign_in == 0)
			{
				ErrorCode = OPERATION_ERROR;
				RespondMsg = "user not login in!\r\n";
				return;
			}
			if (state == RECEIVE_CMD)
			{
				ErrorCode = GO;
				RespondMsg = "go ahead\r\n";
				state = RECEIVE_MAIL;
			}
			else
			{
				if (MSG != ".")
				{
					M_re.content += MSG;
					ErrorCode = 0;
				}
				else
				{
					char path[1024];
					FILE *f;
					_itoa_s(rand(), path, 10);
					int l = strlen(path);
					path[l] = '.';
					path[l + 1] = 't';
					path[2 + l] = 'x';
					path[3 + l] = 't';
					path[4 + l] = 0;
					fopen_s(&f, path, "w");
					fprintf_s(f, "%s", M_re.content.c_str());
					state = RECEIVE_CMD;
					ErrorCode = OK;
					RespondMsg = "Mail Received, named ";
					RespondMsg += path;
					RespondMsg += "\r\n";
					fclose(f);
				}
			}

		}
		void to()
		{
			if (Sign_in == 0)
			{
				ErrorCode = OPERATION_ERROR;
				RespondMsg = "user not login in!\r\n";
				return;
			}
			string mailAddr = Split_msg[2];
			string suffix = ".com";
			string str1 = "@";
			string::size_type idx1 = mailAddr.find(suffix);
			string::size_type idx2 = mailAddr.find(str1);
			if ((idx1 == string::npos) || (idx2 == string::npos))
			{
				ErrorCode = SYNTAX_ERROR;
				RespondMsg = "Wrong mail address!\r\n";
				return;
			}
			M_re.to = mailAddr;
			ErrorCode = OK;
			RespondMsg = "Sender <";
			RespondMsg += Split_msg[2];
			RespondMsg += "> ok!\r\n";
			return;
		}
		void from()
		{
			if (Sign_in == 0)
			{
				ErrorCode = OPERATION_ERROR;
				RespondMsg = "user not login in!\r\n";
				return;
			}
			string mailAddr = Split_msg[2];
			string suffix = ".com";
			string str1 = "@";
			string::size_type idx1 = mailAddr.find(suffix);
			string::size_type idx2 = mailAddr.find(str1);
			if ((idx1 == string::npos) || (idx2 == string::npos))
			{
				ErrorCode = SYNTAX_ERROR;
				RespondMsg = "Wrong mail address!\r\n";
				return;
			}
			M_re.from = mailAddr;
			ErrorCode = OK;
			RespondMsg = "Sender <";
			RespondMsg += Split_msg[2];
			RespondMsg += "> ok!\r\n";
			return;
		}

		void quit()
		{
			Sign_in = NULL;
			ErrorCode = SMTP_QUIT;
			RespondMsg = "Bye!\r\n";
			return;
		}
	};


}
