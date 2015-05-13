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


struct tcpmsg_send
{
	int time;
	int datalen;
	//struct tcp_message tcpmessage;   //���tcpmessage->tcp_seq_number
	int seq_number;
};

struct tcpmsg_rcvd
{
	int datalen;
	int seq_number;
	//struct tcp_message tcpmessage;
};

struct tcplist
{
	tcplist *next;

	unsigned tcp_src_ip : 32;
	unsigned tcp_dst_ip : 32;
	unsigned tcp_src_port : 16;
	unsigned tcp_dst_port : 16;
	int cong_wind;	// ӵ������
	int threshold;	// ��ֵ
	int seq_number;	// ��ǰ������Ӧ����ţ���ʼ��ʱ��0~320���
	int wait_for_ack;	// ��ǰ���ڵȴ�ack�ı�����ţ�Ҳ�����ַ������
	int wait_for_send;	// �ȴ����͵ı������
	int wait_for_fill;	// �ȴ������ֽ������
	int last_rcvd;	// �յ��ı������һ���ֽ����ı��
	int last_read;	// �յ��ı����Ѿ����������һ���ֽ������
	int rcvd_wind;	// ���մ���
	struct tcpmsg_send tcp_msg_send[SEND_STRUCT_SIZE];	// ��ǰTCP�·��ͱ��Ĺ���
	struct tcpmsg_rcvd tcp_msg_rcvd[RCVD_STRUCT_SIZE];	// ��ǰTCP�½��ձ��Ĺ���
	char tcp_buf_send[SEND_BUFFER_SIZE];	// ��ǰTCP�·��ͱ��Ļ�����
	char tcp_buf_rcvd[RCVD_BUFFER_SIZE];	// ��ǰTCP�½��ձ��Ļ�����
	int ack_count;	// ����ack����
	int last_rcvd_ack;	// ��һ��ack��ֵ

	int cwnd;       //���ڴ�С
	unsigned int IP;  //IP
	unsigned short PORT; //�˿ں�
	int Threshold;   //��ֵ
	int MSG_ACK;      // ��ǰ���ڵȴ�ack�ı������
	int MSG_num;    //�Ѿ����͵ı�����
	int MSG_sum;    //һ��Ҫ���͵ı�����
	int send_size;   //�����͵����ݴ�С
	int rec_size;   //�Ѿ��յ���δȷ�ϵ����ݴ�С
	int RcvWindow;   // ���մ��ڵĴ�С
	
};

bool createNodeList();

bool addNode(tcplist* tcp_list);

bool deleteNode(tcplist* p);

struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);

void TCP_new(unsigned int ip_temp, unsigned short port_temp);

void TCP_send();

void TCP_receive();

void TCP_resend();

void TCP_destroy(unsigned int ip_temp, unsigned short port_temp);

void TCP_controller();

int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//������Ҫ�ط���ACK���

int Fastretransmit(int ACK_global);   //�����ش�

int Count_ACK(int ACK_global);   //����ACK������

void initialRTT();			//��ʼ��RTT

float getSampleRTT(int sendtime, int gettime); //��̬���㳬ʱ���

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int dst_ip, unsigned int data_len);