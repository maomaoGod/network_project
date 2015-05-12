#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "MainFrm.h"
#include "TransLayerTools.h"
#include "string.h"

using namespace std;

#define MSS 1024
#define RTT 200
#define Rcvbuffer 1024*1024


struct tcpmsg_send
{
	int ACK;
	int time;
	struct tcp_message tcpmessage;   //���tcpmessage->tcp_seq_number
};



struct tcplist
{
	tcplist* next;
	int cwnd;       //���ڴ�С
	unsigned int IP;  //IP
	unsigned short PORT; //�˿ں�
	int Threshold;   //��ֵ
	int MSG_ACK;      //��ǰ�Ѿ��ж��ٱ��ĵõ���ȷACK
	int MSG_num;    //�Ѿ����͵ı�����
	int MSG_sum;    //һ��Ҫ���͵ı�����
	int send_size;   //�����͵����ݴ�С
	struct tcpmsg_send tcp_msg_send[1024];  //��ǰTCP�·��ʹ����
	struct tcp_message tcp_msg_rec[1024];   //��ǰTCP�½��ܻ�����
	int LastByteRcvd;    //�յ�������ĵı��
	int LastByteRead;    //�Ѿ��ж����յ��ı��ĵõ�ȷ��
	int rec_size;   //�Ѿ��յ���δȷ�ϵ����ݴ�С
	int RcvWindow;   //���ܴ��ڵĴ�С
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


int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//������Ҫ�ط���ACK���

int Fastretransmit(int ACK_global);   //�����ش�

int Count_ACK(int ACK_global);   //����ACK������

void initialRTT();			//��ʼ��RTT

float getSampleRTT(int sendtime, int gettime); //��̬���㳬ʱ���

