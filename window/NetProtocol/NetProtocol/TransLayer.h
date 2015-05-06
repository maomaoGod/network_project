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
	int MSG_num;    //已经发送的报文数
	int cwnd;       //窗口大小
	unsigned int IP;  //IP
	int Threshold;   //阈值
	int count;      //当前已经有多少报文得到正确ACK
	struct tcpmsg tcp_msg[100];
};