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
	//struct tcp_message tcpmessage;   //���tcpmessage->tcp_seq_number
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
	int cong_wind;	// ӵ������
	int threshold;	// ��ֵ
	unsigned int seq_number;	// ��ǰ������Ӧ����ţ���ʼ��ʱ��0~320���
	unsigned int wait_for_ack;	// ��ǰ���ڵȴ�ack�ı�����ţ�Ҳ�����ַ������
	unsigned int wait_for_send;	// �ȴ����͵ı������
	unsigned int wait_for_fill;	// �ȴ������ֽ������
	unsigned int last_rcvd;	// �յ��ı������һ���ֽ����ı��
	unsigned int last_read;	// �յ��ı����Ѿ����������һ���ֽ������
	unsigned int rcvd_wind;	// �Է��Ľ��մ���
	struct tcpmsg_send tcp_msg_send[SEND_STRUCT_SIZE];	// ��ǰTCP�·��ͱ��Ĺ���
	struct tcpmsg_rcvd tcp_msg_rcvd[RCVD_STRUCT_SIZE];	// ��ǰTCP�½��ձ��Ĺ���
	char tcp_buf_send[SEND_BUFFER_SIZE];	// ��ǰTCP�·��ͱ��Ļ�����
	char tcp_buf_rcvd[RCVD_BUFFER_SIZE];	// ��ǰTCP�½��ձ��Ļ�����
	int ack_count;	// ����ack����
	int last_rcvd_ack;	// ��һ���յ���ack��ֵ
	int next_send_ack;	// ��һ��Ӧ�÷��͵�ack��ֵ
	bool send_ack_needed;	// ��������ack
	int wait_for_ack_msg;	// ��ǰ���ڵȴ�ack�ı��ı�ţ��������±�
	int wait_for_fill_msg;	// ��ǰ���ڵȴ����ı��ı�ţ��������±�
	int last_rcvd_msg;	// �յ��ı������һ�����ı��
	int last_read_msg;	// �յ��ı����Ѿ����������һ�����ı��
	int status;	// ӵ������״̬
	int tcp_established_syn_seq;	// �Է�������syn���ڱ��ı��
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

int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum);//������Ҫ�ط���ACK���

int Fastretransmit(int ACK_global);   //�����ش�

int Count_ACK(int ACK_global);   //����ACK������

void initialRTT();			//��ʼ��RTT

float getSampleRTT(int sendtime, int gettime); //��̬���㳬ʱ���

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);

bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seg_number);

int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place);
