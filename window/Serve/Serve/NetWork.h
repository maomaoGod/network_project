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
			if (Function.find(CMD)==Function.end()){
				ErrorCode = BAD_REQUEST;
				return;
			}
			try{
				//ע��1���������ʹ��this->���������error C2171: ��*��:
				//��void (__thiscall CUtil::* )(int)�����͵Ĳ������Ƿ��Ĵ���
				//ע��2���������ʹ��*�����ã�����ʵ�ֺ������ã�
				//�������error C2064: ������Ϊ���� 1 �������ĺ����Ĵ���
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
		/*
		void DELETE(vector<string> data){

		}*/

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
			//Function["DELETE"] = Function["delete"] = &AppLayerHttp::DELETE;
			/**@brief ���ڷ�����֧�ֵ����󷽷���Ϣ������,
			  Map it to 6*/
			Function["OPTIONS"] = Function["options"] = &AppLayerHttp::OPTIONS;
			/**@brief Web����������Http�������ͷ�������,
			  Map it to 7*/
			//Function["TRACE"] = Function["trace"] = &AppLayerHttp::TRACE;
			/**@brief ���ĵ�������ǰδʵ�ֵ�һ��������
			  Ԥ�����������, Map it to 8*/
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
	*@brief TCP��UDP�Ĺ������֣�֧�ֿɿ�����Ͳ��ɿ������ʵ��
	*@author  ACM2012
	*@note
	*Msg -> ����
	*  head -> ����ͷ
	*  data -> ������
	*  IP -> Ӧ�ò��HOST
	*  MORE
	*/
	/** @file
	* @brief TCP UDP������
	* @author ACM2012
	* @date 2015/04/19
	* @version 0.1
	* @note
	* ������CSocket�ķ�������TCP���ӣ�����ʵ��ʱ�ֱ�ʵ��TCP��UDP�Ĺ���
	* ���Ƚ���Ƚ��ѵ�TCP���ӣ���TCP�Ĺ���ʵ�ֺ�������ȫUDP�Ĳ���
	* ����ʵ�ֲ��ְ�������ͷ�Ĺ��죬���ӵ�ʵ�֣��ɿ�������ƣ��������Ƶ�
	* ���嶨���ں�����ϸ˵��
	* ���ļ��е�UDP����δ����,��ʵ��TCP��������ȫ��Ӧ����.
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
		*��׼���캯��
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
		/**@brief ���캯��*/
		MyUDP();
		/**@brief ��������*/
		virtual ~MyUDP();

	protected:
		typedef Byte SOCKADDR;
		typedef Byte SOCKADDR_IN;

		SOCKADDR_IN m_clientAddr;
		
		int ReceiveFrom(Byte *Buf, int len, SOCKADDR* client_addr, int *addr_len, int mode){

		}
		/**@brief ��дOnReceive����*/
		virtual void OnReceive(int nErrorCode)
		{
			Byte recBuf[1024];
			/**@brief Socket��ַ���ȣ����Լ̳���CAsyncSocket*/
			int len = sizeof(SOCKADDR_IN);
			/**@brief ���buffer*/
			//ZeroMemory(recBuf);
			/**@brief ����ReceiveFrom��������������*/
			int recBytes = ReceiveFrom(recBuf, 1023, (SOCKADDR*)&m_clientAddr, &len, 0);
			
			/**@brief �жϽ������ݵ����*/
			if (recBytes == 0)
			{
				/**@brief ��ʾUDPδ�յ���Ϣ*/
			}
			else if (recBytes == SOCKET_ERROR)
			{
				/**@brief ��ʾ���ܴ���*/
			}
			else
			{
				/**@brief �յ����ݣ������ݸ���*/
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
		    //��headָ�뿪ʼ��ͷ2λ��Դ�˿ںţ�3-4λ��Ŀ�Ķ˿ںţ�5-6λ���յ������ݵĳ��ȣ�7-8λ�ż����
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
		*@brief �������������ɵ�message���ͳ�ȥ
		*@author  ACM2012
		*@return ���ͳɹ�����true������ʧ�ܷ���false
		*@note
		*@remarks
		*/
		bool UDP_Send(){
			//���û��ຯ��sender���������ɵı��ķ���ȥ
			if (Sender())
				return true;
			else return false;
		}
	};

	/**
	* @class
	* @brief �����װTCPЭ�鷽��.
	* @author ACM2012
	* @note
	* �����������Ĺ��캯����������������ʵ��TCPЭ��.
	* ���캯��MyTCPӦ���������̵ĵ�ַ���˿ں�
	* ��������������Ҫ����
	* ���Ӻ���TCP_Conn Ӧ�����TCPЭ�����������
	* �����Ӻ���TCP_AcceptӦ���ܹ���ȷ�ĺ��������ӵĽ��̽���ͨѶ
	* ���Ӻ���TCP_ListenӦ�������ⲿ���󣬵��յ�����ʱ�ٵ���������Ա������
	* ���ú���TCP_SetOptӦ������TCP�и���������籨��ͷ�в��ֲ����Լ�ӵ��������ز���
	* ��ֱ���޸Ķ�Ӧ������Ҫ�ǳ��ڰ�ȫ����-------->>>>>����ʵ�ֿɲ�ʹ�ô˺���������
	* ���ͺ���Simple_Sendʵ�ּ򵥵ķ���һ�α��ĵĹ��ܣ�����Ŀɿ�Э����TCP_Send��ʵ��
	* ���ͺ���TCP_Sendʵ�ַ��͹��ܣ�ͬʱʵ����Ӧ�ÿ��ǵ��ɿ�����ĸ�����ƣ���ȷ���ش���
	* ���պ���TCP_Receiveʵ�ֽ������ݵĹ��ܣ�ע����������������������Ĺ��ܣ�
	* ͬʱ��Ȼ��Ҫ���ǿɿ����������
	* �������TCP_DisConn ��ɽ�����ӵĹ���
	* �ඨ�岢��������ֻ��������˼·�����
	*/

	class MyTCP :MySocket
	{
		/** @brief ���캯��*/
		MyTCP();
		/** @brief ��������*/
		~MyTCP();
	public:
		/** @brief ���Ӻ���,
		* @note
		* ����һ�����ӣ�Ӧ���������ӳɹ���������Ϣ
		* �˴�Ӧ������������ֵĿɿ�����
		* ����SYN,ACK�ȱ�־λ������
		*/
		bool TCP_Conn();

		/** @brief �����Ӻ���,
		* @note
		* Ӧ���ܹ���ȷ�ĺ��������ӵĽ��̽�������
		*/
		void TCP_Accept();

		/** @brief ��������,
		* @note
		* �����ⲿ���󣬵��յ�����ʱ�ٵ���������Ա������
		*/
		void TCP_Listen();

		/** @brief ���ͺ���,
		* @note
		* ʵ�ַ��͹��ܣ�ͬʱʵ����Ӧ�ÿ��ǵ��ɿ�����ĸ������
		* ע���ش��Ĳ������綨ʱ�Ȳ�Ӧʹ�þֲ�����
		* �ɿ������������ʱ�ش�����������ACK�ش�
		* �ش�ʹ�û���N������
		*/
		void TCP_Send();

		/** @brief ���պ���,
		* @note
		* ʵ�ֽ������ݵĹ���
		* ע��ɿ������µ�ȷ�ϱ��ķ���
		* ��Ҫ�������ʧ�ķ����������ۻ�ȷ�ϵ�
		*/
		void TCP_Receive();

		/** @brief ������Ӻ���,
		* @note
		* ��ɽ�����ӵĹ���
		*/
		void TCP_DisConn();
	private:
		/** @brief ����ͷ�ṹ
		* @note
		*����ѡ��ָ�����Ҫ�������
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
		/** @brief �򵥷���
		* @note
		* �򵥷��ͱ��ģ��û���Ӧ����ʹ�������������Ϊ����û�пɿ�����
		*/
		void Simple_Send();
		/** @brief �򵥽���
		* @note
		* �򵥽��ձ��ģ��û���Ӧ����ʹ�������������Ϊ����û�пɿ�����
		*/
		void Simple_Receive();

		/** @brief ����ṹ�壬��������ӵ�����ƵĲ���
		* @note
		* SS:������״̬
		* CA��ӵ������״̬
		* RTT��һ������ʱ��
		* MSS�����ͷ�����շ�Э������Ķγ���
		* CongWin��ӵ������
		* Threshold����ֵ
		* redundancy_ACK������ACK����
		* state����ǰ״̬
		* Time_out����ʱ
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

		/** @brief ӵ�����ƺ���,
		* @note
		* (1)�յ�δȷ�����ݵ�ACK��
		* ������������״̬��ÿ��һ��RTT��ӵ�����ڷ�������ӵ�����ڳ�����ֵ��״̬�ı�Ϊ"ӵ������"
		* ������ӵ������״̬��ÿ��һ��RTT��ӵ����������һ��MSS
		* (2)����������
		* ����3������ACK������ӵ�����ں���ֵ����ΪCongWin��һ�룬����״̬Ϊ��ӵ�����⡱
		* ���ɳ�ʱ��������ֵ��Ϊӵ�����ڵ�һ�룬ӵ����������Ϊ1��MSS������״̬Ϊ����������
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
