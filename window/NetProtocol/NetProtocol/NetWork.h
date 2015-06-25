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

namespace NetWork{

#define FTP_PORT 7600
#define root_power 0
#define user_power 3
#define Sign_SU 200
#define Sign_Fail 300
#define OP_OK 120
#define OP_Fail 302
#define QUIT_EXIT 303
#define Rev_OK 320

	/**
	*@class AppLayerFtp NetWork.h "Serve/AppLayerFtp.h"
	*@brief FtpЭ��ͻ��˵�ʵ��
	*@author ACM2012
	*@date 2015/06/06
	*@version 1.1
	*@note
	* ʵ��FtpЭ��������Ĺ��ܣ�ʵ���ļ�����
	*/
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

		/**
		* @brief
		* @param [in] <RevData> string��
		* @author ACM2012
		* @return ��
		* @note
		*  ��ȡ���ݣ������Կո�ָ�����
		*/
		void GetData(string RevData){
			STR::Split(RevData, &data, ' ');
		}
		/**
		* @brief
		* @author ACM2012
		* @return string�����ݣ�
		* @note
		*  ��ȡ����
		*/
		string GetCMD(){
			return data[0];
		}

		/**
		* @brief
		* @author ACM2012
		* @return int������
		* @note
		*  ��ȡ��Ӧ����
		*/
		int GetCode(){
			return ErrorCode;
		}

		/**
		* @brief
		* @author ACM2012
		* @return string������
		* @note
		*  ��ȡӦ����
		*/
		string GetResMsg(){
			return RespondMsg;
		}
		//,vector<string> args
		/**
		* @brief
		* @param [in] <CMD> string��
		* @author ACM2012
		* @return ��
		* @note
		*  ��������
		*/
		void DealWith(string CMD){
			/**@brief DON'T HAVE THIS CMD return BAD_REQUEST*/
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
		///<CMD �� user + pass
		void SIGNIN(vector<string> data){//char path[1024];
			if (Sign_in != NULL) delete Sign_in;///<����
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �˳���¼
		*/
		void QUIT(vector<string> data){
			delete Sign_in;
			Sign_in = NULL;
			ErrorCode = QUIT_EXIT;
			RespondMsg = "Sign Out!";
			return;
		}
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �˳���¼
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* ����һ��ָ����Ŀ¼
		*/
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
		};
			/**
			* @brief
			* @param [in] <data> vector<string>��
			* @author ACM2012
			* @return ��
			* @note
			* ɾ���ļ�
			*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �ϴ��ļ�
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �����ļ�
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �����û�
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* ɾ���û�
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* �����ļ�
		*/
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
		/**
		* @brief
		* @param [in] <data> vector<string>��
		* @author ACM2012
		* @return ��
		* @note
		* FtpЭ���ʼ��
		*/
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
	/**
	*@class SMTP NetWork.h "HustNet/NetWork.h"
	*@brief How the SMTP protocol works
	*@author  ACM2012
	*@note
	* SmtpЭ���ʵ�֣��ʼ�����Э������
	*/
	class SMTP
	{
	public:
		/**
		* @brief DNSworker���캯��
		* @author ACM2012
		* @return ��
		* @note
		*/
		SMTP()
		{
			memset(&M_re, 0, sizeof(M_re));
			Sign_in = 0;
			state = RECEIVE_CMD;
			ErrorCode = 0;
			RespondMsg = "";
		}
		/**
		* @brief DNSworker��������
		* @author ACM2012
		* @return ��
		* @note
		*/
		~SMTP()
		{}
		/**
		* @brief ��ȡ��Ӧ����
		* @author ACM2012
		* @return ��
		* @note
		*/
		int GetCode()
		{
			return ErrorCode;
		}
		/**
		* @brief ��ȡ������
		* @author ACM2012
		* @return string������
		* @note
		*/
		string GetMsg()
		{
			return RespondMsg;
		}
		/**
		* @brief �������
		* @author ACM2012
		* @return ��
		* @note
		*  SMTPЭ����������У�
		* auth , noop��help��data��quit��mail��rcpt��rset
		* �����������8�֣��򷵻ء�Unknown Command��
		*/
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
		/**
		* @brief �ʼ��Ľṹ
		* @author ACM2012
		* @note
		* �ṹ�����ռ��ˡ������ˡ��ʼ�����
		*/
		struct Mail
		{
			string to;
			string from;
			string content;
		}M_re;
	private:
		/**
		* @brief ���·����ʼ�
		* @author ACM2012
		* @return ��
		* @note
		*/
		void rset()
		{
			memset(&M_re, 0, sizeof(M_re));
			ErrorCode = OK;
			RespondMsg = "Clear the data\r\n";
		}
		/**
		* @brief ��¼�ʼ�ϵͳ
		* @author ACM2012
		* @return ��
		* @note
		*/
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
		/**
		* @brief ����հ�
		* @author ACM2012
		* @return ��
		* @note
		*/
		void noop()
		{
			ErrorCode = OK;
			RespondMsg = "OK!\r\n";
			return;
		}
		/**
		* @brief �����ļ���ʵ��
		* @author ACM2012
		* @return ��
		* @note
		*/
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
		/**
		* @brief �����������ݵ�ʵ��
		* @author ACM2012
		* @return ��
		* @note
		*/
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
		/**
		* @brief �ռ���
		* @author ACM2012
		* @return ��
		* @note
		*/
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
		/**
		* @brief ������
		* @author ACM2012
		* @return ��
		* @note
		*/
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
		/**
		* @brief �˳���¼��ʵ��
		* @author ACM2012
		* @return ��
		* @note
		*/
		void quit()
		{
			Sign_in = NULL;
			ErrorCode = SMTP_QUIT;
			RespondMsg = "Bye!\r\n";
			return;
		}
	};


}
