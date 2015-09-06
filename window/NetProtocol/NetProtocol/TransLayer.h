// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 TRANSLAYER_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// TRANSLAYER_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
//#ifdef TRANSLAYER_EXPORTS
//#define TRANSLAYER_API __declspec(dllexport)
//#else
//#define TRANSLAYER_API __declspec(dllimport)
//#endif

/**@file
*@brief tcp相关协议实现头文件
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
#pragma once
#include "stdafx.h"
#include "string.h"

#include "TransLayerTools.h"
#pragma comment (lib, "TransLayerTools.lib")

using namespace std;

//--------------------------------------------------------------------------------
#define TRANSTOIP       WM_USER+1000
#define IPTOLINK        WM_USER+1001
#define LINKSEND        WM_USER+1002
#define TRANSTOAPP      WM_USER+1003
#define APPTOTRANS	    WM_USER+1004
#define IPTOTRANS       WM_USER+1005
#define LINKTOIP        WM_USER+1006
#define	APPSEND			WM_USER+1007
#define	SOCKSTATEUPDATE	WM_USER+1008

#define  SOCKCONNECT		200
#define  SOCKBIND           201
#define  SOCKLISTEN         202
#define  SOCKSEND           203
#define  SOCKSENDTO         204
#define  SOCKRECEIVE        205
#define  SOCKRECEIVEFROM	206
#define  SOCKCLOSE          207
#define  SOCKACCEPT         208
//---------------------------------------------------------------------------------

#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17

#define MSS 1024
#define INITIAL_RCVD_WIND 65535
#define SEND_STRUCT_SIZE 1024
#define RCVD_STRUCT_SIZE 1024
#define SEND_BUFFER_SIZE (1024*1024)
#define RCVD_BUFFER_SIZE (1024*1024)
#define INITIAL_THRESHOLD (65*1024) // 65KB
#define RANDOM_SEQ_NUMBER_MODULE 321
#define MAX_PORT 65536
#define RESEND_COUNTS 1000000000

#define CONG_SS 0
#define CONG_CA 1

#define LINK_CONNECTING 0
#define LINK_WAIT_FOR_SYN 1
#define LINK_GOT_SYN 2
#define LINK_CONNECTED 3
#define LINK_WAIT_FOR_SYNACK 4
#define LINK_GOT_SYNACK 5
#define LINK_CONNECT_BIDIR 6
#define LINK_FINISHING 7
#define LINK_WAIT_FOR_FINACK 8
#define LINK_SELF_HALF_OPEN 9
#define LINK_FINISHED 10
#define LINK_PEER_HALF_OPEN 11
#define LINK_CONNECT_DESTROYED 12
#define LINK_CONNECT_DESTROYING 13
#define LINK_WAIT_FOR_DESACK 14
#define LINK_CONNECT_LOSE 15
#define wnd_SR 100			//小于等于SEND_STRUCT_SIZE/2即可

//--------------------------------------------------------------------------------------------
struct SockStruct_SocketLayer {
	unsigned short  dstport;   //目的端口号
	unsigned short  srcport;   //源端口号
	unsigned short  funcID; //socket操作码
	unsigned short	padding;		// blank
	int    datalength;             //数据长度
	char srcip[20];                //原地址ip
	char dstip[20];                //目标地址ip
	char *data;             //数据
};

struct Msg_NetLayer{                     ///<数据
	unsigned int ih_sport;		///< 32位源端口号
	unsigned int ih_dport;		///< 32位目的端口号
	unsigned int sip;			///< 32位源IP
	unsigned int dip;			///< 32位目的IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< 上层协议类型
};
//--------------------------------------------------------------------------------------------

/**
*@class <tcpmsg_send>
*@brief 当前TCP下发送报文管理结构
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
struct tcpmsg_send
{
	int time;
	int datalen;
	//struct tcp_message tcpmessage;   //序号tcpmessage->tcp_seq_number
	int seq_number;
};
/**
*@class <tcpmsg_rcvd>
*@brief 当前TCP下接收报文管理结构
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
struct tcpmsg_rcvd
{
	int datalen;
	int seq_number;
	bool handin;
	//struct tcp_message tcpmessage;
};
/**
*@class <tcplist>
*@brief tcp连接链表
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*@note 记录所有TCP连接下的所需的相关变量，发送缓冲区，接收缓冲区等等
*/
struct tcplist
{
	struct tcplist *next;
	unsigned tcp_src_ip : 32;
	unsigned tcp_dst_ip : 32;
	unsigned tcp_src_port : 16;
	unsigned tcp_dst_port : 16;
	int cong_wind;	// 拥塞窗口
	int threshold;	// 阀值
	unsigned int seq_number;	// 当前发送所应在序号，初始化时在0~320随机
	unsigned int wait_for_ack;	// 当前正在等待ack的报文序号，也就是字符流编号
	unsigned int wait_for_send;	// 等待发送的报文序号
	unsigned int wait_for_fill;	// 等待填充的字节流序号
	unsigned int last_rcvd;	// 收到的报文最后一个字节流的编号
	unsigned int last_read;	// 收到的报文已经交付的最后一个字节流编号
	unsigned int rcvd_wind;	// 对方的接收窗口
	struct tcpmsg_send tcp_msg_send[SEND_STRUCT_SIZE];	// 当前TCP下发送报文管理
	struct tcpmsg_rcvd tcp_msg_rcvd[RCVD_STRUCT_SIZE];	// 当前TCP下接收报文管理
	char tcp_buf_send[SEND_BUFFER_SIZE];	// 当前TCP下发送报文缓冲区
	char tcp_buf_rcvd[RCVD_BUFFER_SIZE];	// 当前TCP下接收报文缓冲区
	int ack_count;	// 冗余ack计数
	int last_rcvd_ack;	// 上一个收到的ack的值
	int next_send_ack;	// 下一个应该发送的ack的值
	bool send_ack_needed;	// 立即发送ack
	int wait_for_ack_msg;	// 当前正在等待ack的报文编号，是数组下标
	int wait_for_fill_msg;	// 当前正在等待填充的报文编号，是数组下标
	int last_rcvd_msg;	// 收到的报文最后一个报文编号
	int last_read_msg;	// 收到的报文已经交付的最后一个报文编号
	int cong_status;	// 拥塞控制状态
	int tcp_established_syn_seq;	// 对方发来的syn所在报文编号
	int connect_status;	// TCP连接状态
	int receive_time; //当前收到的最后一个报文的时间
	int resend_count;  //重传计数
	bool waiting_for_ack; //是否正在等待ack
};

//// 此类是从 TransLayer.dll 导出的
//class TRANSLAYER_API CTransLayer {
//public:
//	// TODO:  在此添加您的方法。
//	static bool createNodeList();
//
//	static bool addNode(tcplist* tcp_list);
//
//	static bool deleteNode(tcplist* p);
//
//	static struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);
//
//	static void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port, int status);
//
//	static void TCP_send(struct SockStruct_SocketLayer data_from_applayer);
//
//	static void TCP_receive(struct Msg_NetLayer data_from_netlayer);
//
//	static void TCP_resend();
//
//	static void TCP_close(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);
//
//	void TCP_controller();
//
//	static void initialRTT();			//初始化RTT
//
//	static float getSampleRTT(int sendtime, int gettime); //动态计算超时间隔
//
//	static void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);
//
//	static void UDP_Send2IP(struct SockStruct_SocketLayer data_from_applayer, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);
//
//	static bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seg_number);
//
//	static int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place);
//
//	static void port_listen(unsigned short port);
//
//	static bool check_listening(unsigned short port);
//
//	static void fill_new_tcplist(struct tcplist *new_tcp);
//
//	static void Temp_Send_ACK(struct tcplist *single_tcp);
//
//	static void time_over();
//
//};
//
//extern TRANSLAYER_API int nTransLayer;
//
//TRANSLAYER_API int fnTransLayer(void);
