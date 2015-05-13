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
#define Rcvbuffer 1024*1024
#define SEND_BUFFER_SIZE 1024
#define RECEIVE_BUFFER_SIZE 1024
#define INITIAL_THRESHOLD 65*1024 // 65KB
#define RANDOM_SEQ_NUMBER_MODULE 321


struct tcpmsg_send
{
	int time;
	struct tcp_message tcpmessage;   //序号tcpmessage->tcp_seq_number
};



struct tcplist
{
	tcplist *next;
	int cwnd;       //窗口大小
	unsigned int IP;  //IP
	unsigned short PORT; //端口号
	int Threshold;   //阈值
	int MSG_ACK;      //当前已经有多少报文得到正确ACK
	int MSG_num;    //已经发送的报文数
	int MSG_sum;    //一共要发送的报文数
	int send_size;   //待发送的数据大小
	struct tcpmsg_send tcp_msg_send[SEND_BUFFER_SIZE];  //当前TCP下发送存放区
	struct tcp_message tcp_msg_rec[RECEIVE_BUFFER_SIZE];   //当前TCP下接收缓冲区
	int LastByteRcvd;    //收到的最后报文的编号
	int LastByteRead;    //已经有多少收到的报文得到确认
	int rec_size;   //已经收到但未确认的数据大小
	int RcvWindow;   //接受窗口的大小
	int seq_number;	// 当前发送所应在序号，初始化时在0~320随机
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deletenode(tcplist* p);

struct tcplist *getNode(unsigned int ip, unsigned short port);

void TCP_new(unsigned int ip_temp, unsigned short port_temp);

void TCP_send();

void TCP_receive();

void TCP_resend();

void TCP_destroy(unsigned int ip_temp, unsigned short port_temp);

void TCP_controller();

void mescopy(struct tcp_message tcp_msg_a, struct tcp_message tcp_msg_b);

int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//返回需要重发的ACK序号

int Fastretransmit(int ACK_global);   //快速重传

int Count_ACK(int ACK_global);   //冗余ACK计数器

void initialRTT();			//初始化RTT

float getSampleRTT(int sendtime, int gettime); //动态计算超时间隔

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int dst_ip, unsigned int data_len);