#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "MainFrm.h"
#include "TransLayerTools.h"
#include "string.h"

using namespace std;

#define MSS 1024
#define RTT 200

struct tcpmsg
{
	int ACK;
	int time;
	struct tcp_message tcpmessage;   //序号tcpmessage->tcp_seq_number
};

struct tcplist
{
	tcplist* next;
	int MSG_num;    //已经发送的报文数
	int cwnd;       //窗口大小
	unsigned int IP;  //IP
	unsigned short PORT; //端口号
	int Threshold;   //阈值
	int count;      //当前已经有多少报文得到正确ACK
	struct tcpmsg tcp_msg[1024];  //当前TCP下发送的报文
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deletenode(tcplist* p);

struct tcplist *getNode(unsigned int ip, unsigned short port);

void TCP_new();

void TCP_send();

void TCP_receive();

void TCP_resend();

void TCP_destroy();

void TCP_controller();

void mescopy(struct tcp_message tcp_msg_a, struct tcp_message tcp_msg_b);