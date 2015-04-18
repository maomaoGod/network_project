#include "map"
#include "Tools.h"


namespace NetWork{
	/*HOW IT WORKS
	
	*/
	class AppLayerHttp{
	public:
		string Log;
		AppLayerHttp(){
			Http_init();//init the map
			Listen_launch();//launch a listen thread
		}
		~AppLayerHttp(){
			delete &Function;//remove the map
		}
		/*name: Getpath
		  function: analysis the path send by 
		*/
		string Getpath(){
			string path;
			//div by some char
			//
			return path;
		}
		//map the CMD to the real
		void DealWith(string CMD, vector<string> args){
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
		/*the map struct*/
		typedef void(AppLayerHttp::*DealWithFunciton)(vector<string> data);
		map<string, DealWithFunciton> Function;
		string path;
		string RespondMsg;
		/* the function we choose to deal with the data request*/
		void GET(vector<string> data){

		}
		void HEAD(vector<string> data){

		}
		void POST(vector<string> data){

		}
		void PUT(vector<string> data){

		}
		void DELETE(vector<string> data){

		}
		void OPTIONS(vector<string> data){

		}
		void TRACE(vector<string> data){

		}
		void CONNECT(vector<string> data){

		}
		
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
		}


	};
}
