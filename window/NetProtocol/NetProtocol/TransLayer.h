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
	int seq;
	int time;
};

struct tcplist
{
	tcplist* next;
	int MSG_num;    //�Ѿ����͵ı�����
	int cwnd;       //���ڴ�С
	unsigned int IP;  //IP
	int Threshold;   //��ֵ
	int count;      //��ǰ�Ѿ��ж��ٱ��ĵõ���ȷACK
	struct tcpmsg tcp_msg[100];
};