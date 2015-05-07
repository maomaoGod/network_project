#pragma once
#include "stdafx.h"
#include "Tools.h"
#include "MainFrm.h"
#include "TransLayerTools.h"

using namespace std;

#define MSS 1024
#define RTT 200

struct tcpmsg
{
	int ACK;
	int time;
	struct tcp_message tcpmessage;   //���tcpmessage->tcp_seq_number
};

struct tcplist
{
	tcplist* next;
	int MSG_num;    //�Ѿ����͵ı�����
	int cwnd;       //���ڴ�С
	unsigned int IP;  //IP
	int Threshold;   //��ֵ
	int count;      //��ǰ�Ѿ��ж��ٱ��ĵõ���ȷACK
	struct tcpmsg tcp_msg[1024];  //��ǰTCP�·��͵ı���
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deletenode(tcplist* p);

tcplist *getNode(unsigned int ip);

void TCP_controller();