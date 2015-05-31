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
#define INITIAL_RCVD_WIND 1024*1024
#define SEND_STRUCT_SIZE 1024
#define RCVD_STRUCT_SIZE 1024
#define SEND_BUFFER_SIZE 1024*1024
#define RCVD_BUFFER_SIZE 1024*1024
#define INITIAL_THRESHOLD 65*1024 // 65KB
#define RANDOM_SEQ_NUMBER_MODULE 321

#define CONG_SS 0
#define CONG_CA 1


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
	tcplist *next;
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
	int status;	// 拥塞控制状态
	int tcp_established_syn_seq;	// 对方发来的syn所在报文编号
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deleteNode(tcplist* p);

struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_send(struct sockstruct data_from_applayer);

void TCP_receive(struct Msg data_from_netlayer);

void TCP_resend();

void TCP_destroy(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_controller();

int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//返回需要重发的ACK序号

int Fastretransmit(int ACK_global);   //快速重传

int Count_ACK(int ACK_global);   //冗余ACK计数器

void initialRTT();			//初始化RTT

float getSampleRTT(int sendtime, int gettime); //动态计算超时间隔

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);

bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seg_number);

int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place);
