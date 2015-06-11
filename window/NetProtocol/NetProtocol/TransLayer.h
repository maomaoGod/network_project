#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "MainFrm.h"
#include "TransLayerTools.h"
#include "string.h"

using namespace std;

#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17

#define MSS 1024
#define INITIAL_RCVD_WIND (1024*1024)
#define SEND_STRUCT_SIZE 1024
#define RCVD_STRUCT_SIZE 1024
#define SEND_BUFFER_SIZE (1024*1024)
#define RCVD_BUFFER_SIZE (1024*1024)
#define INITIAL_THRESHOLD (65*1024) // 65KB
#define RANDOM_SEQ_NUMBER_MODULE 321
#define MAX_PORT 65536

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

struct tcpmsg_send
{
	int time;
	int datalen;
	//struct tcp_message tcpmessage;   //序号tcpmessage->tcp_seq_number
	int seq_number;
};

struct tcpmsg_rcvd
{
	int datalen;
	int seq_number;
	bool handin;
	//struct tcp_message tcpmessage;
};

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
};

struct stopandwait
{
	int time;               //记录发送报文的时间
	struct sockstruct send_buf[SEND_STRUCT_SIZE];   //发送缓冲区
	int last_waitforsend_msg;     //当前缓冲区中正待被发送的报文下标
	int last_send_msg;            //当前缓冲区中将要发送的最后一个报文的下标
	struct tcp_message last_send;   //上一个发送出去的报文
	int last_ack;
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deleteNode(tcplist* p);

struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port, int status);

void TCP_send(struct sockstruct data_from_applayer);

void TCP_receive(struct Msg data_from_netlayer);

void TCP_resend();

void TCP_close(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_controller();

//int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//返回需要重发的ACK序号
//
//int Fastretransmit(int ACK_global);   //快速重传
//
//int Count_ACK(int ACK_global);   //冗余ACK计数器

void initialRTT();			//初始化RTT

float getSampleRTT(int sendtime, int gettime); //动态计算超时间隔

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);

void UDP_Send2IP(struct sockstruct data_from_applayer, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);

bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seg_number);

int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place);

int wait_for_handshaking_ack(struct tcplist *tcp);

void port_listen(unsigned short port);

bool check_listening(unsigned short port);

void fill_new_tcplist(struct tcplist *new_tcp);

