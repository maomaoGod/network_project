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
	struct tcp_message tcpmessage;   //ÐòºÅtcpmessage->tcp_seq_number
};

struct tcplist
{
	tcplist* next;
	int MSG_num;    //ÒÑ¾­·¢ËÍµÄ±¨ÎÄÊý
	int cwnd;       //´°¿Ú´óÐ¡
	unsigned int IP;  //IP
	unsigned short PORT; //¶Ë¿ÚºÅ
	int Threshold;   //ãÐÖµ
	int count;      //µ±Ç°ÒÑ¾­ÓÐ¶àÉÙ±¨ÎÄµÃµ½ÕýÈ·ACK
	struct tcpmsg tcp_msg[1024];  //µ±Ç°TCPÏÂ·¢ËÍµÄ±¨ÎÄ
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

void Fastretransmit(Receive_ACK_ID);

int Getrwnd(int RcvBuffer);

void FlowControl(Rcv_Window,Last_Rcv_ACK);//滑动窗口流量控制