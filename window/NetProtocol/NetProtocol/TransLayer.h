// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� TRANSLAYER_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// TRANSLAYER_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
//#ifdef TRANSLAYER_EXPORTS
//#define TRANSLAYER_API __declspec(dllexport)
//#else
//#define TRANSLAYER_API __declspec(dllimport)
//#endif

/**@file
*@brief tcp���Э��ʵ��ͷ�ļ�
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
#pragma once
#include "stdafx.h"
#include "string.h"

#include "TransLayerTools.h"
#pragma comment (lib, "TransLayerTools.lib")

using namespace std;

//--------------------------------------------------------------------------------
#define TRANSTOIP       WM_USER+1000
#define IPTOLINK        WM_USER+1001
#define LINKSEND        WM_USER+1002
#define TRANSTOAPP      WM_USER+1003
#define APPTOTRANS	    WM_USER+1004
#define IPTOTRANS       WM_USER+1005
#define LINKTOIP        WM_USER+1006
#define	APPSEND			WM_USER+1007
#define	SOCKSTATEUPDATE	WM_USER+1008

#define  SOCKCONNECT		200
#define  SOCKBIND           201
#define  SOCKLISTEN         202
#define  SOCKSEND           203
#define  SOCKSENDTO         204
#define  SOCKRECEIVE        205
#define  SOCKRECEIVEFROM	206
#define  SOCKCLOSE          207
#define  SOCKACCEPT         208
//---------------------------------------------------------------------------------

#define PROTOCOL_TCP 6
#define PROTOCOL_UDP 17

#define MSS 1024
#define INITIAL_RCVD_WIND 65535
#define SEND_STRUCT_SIZE 1024
#define RCVD_STRUCT_SIZE 1024
#define SEND_BUFFER_SIZE (1024*1024)
#define RCVD_BUFFER_SIZE (1024*1024)
#define INITIAL_THRESHOLD (65*1024) // 65KB
#define RANDOM_SEQ_NUMBER_MODULE 321
#define MAX_PORT 65536
#define RESEND_COUNTS 1000000000

#define CONG_SS 0
#define CONG_CA 1

#define LINK_CONNECTING 0
#define LINK_WAIT_FOR_SYN 1
#define LINK_GOT_SYN 2
#define LINK_CONNECTED 3
#define LINK_WAIT_FOR_SYNACK 4
#define LINK_GOT_SYNACK 5
#define LINK_CONNECT_BIDIR 6
#define LINK_FINISHING 7
#define LINK_WAIT_FOR_FINACK 8
#define LINK_SELF_HALF_OPEN 9
#define LINK_FINISHED 10
#define LINK_PEER_HALF_OPEN 11
#define LINK_CONNECT_DESTROYED 12
#define LINK_CONNECT_DESTROYING 13
#define LINK_WAIT_FOR_DESACK 14
#define LINK_CONNECT_LOSE 15
#define wnd_SR 100			//С�ڵ���SEND_STRUCT_SIZE/2����

//--------------------------------------------------------------------------------------------
struct SockStruct_SocketLayer {
	unsigned short  dstport;   //Ŀ�Ķ˿ں�
	unsigned short  srcport;   //Դ�˿ں�
	unsigned short  funcID; //socket������
	unsigned short	padding;		// blank
	int    datalength;             //���ݳ���
	char srcip[20];                //ԭ��ַip
	char dstip[20];                //Ŀ���ַip
	char *data;             //����
};

struct Msg_NetLayer{                     ///<����
	unsigned int ih_sport;		///< 32λԴ�˿ں�
	unsigned int ih_dport;		///< 32λĿ�Ķ˿ں�
	unsigned int sip;			///< 32λԴIP
	unsigned int dip;			///< 32λĿ��IP
	int datelen;
	char data[2048];
	unsigned int protocol;		///< �ϲ�Э������
};
//--------------------------------------------------------------------------------------------

/**
*@class <tcpmsg_send>
*@brief ��ǰTCP�·��ͱ��Ĺ���ṹ
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
struct tcpmsg_send
{
	int time;
	int datalen;
	//struct tcp_message tcpmessage;   //���tcpmessage->tcp_seq_number
	int seq_number;
};
/**
*@class <tcpmsg_rcvd>
*@brief ��ǰTCP�½��ձ��Ĺ���ṹ
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*/
struct tcpmsg_rcvd
{
	int datalen;
	int seq_number;
	bool handin;
	//struct tcp_message tcpmessage;
};
/**
*@class <tcplist>
*@brief tcp��������
*@author ACM2012
*@date 2015/6/1
*@version <0.1>
*@note ��¼����TCP�����µ��������ر��������ͻ����������ջ������ȵ�
*/
struct tcplist
{
	struct tcplist *next;
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
	int cong_status;	// ӵ������״̬
	int tcp_established_syn_seq;	// �Է�������syn���ڱ��ı��
	int connect_status;	// TCP����״̬
	int receive_time; //��ǰ�յ������һ�����ĵ�ʱ��
	int resend_count;  //�ش�����
	bool waiting_for_ack; //�Ƿ����ڵȴ�ack
};

//// �����Ǵ� TransLayer.dll ������
//class TRANSLAYER_API CTransLayer {
//public:
//	// TODO:  �ڴ�������ķ�����
//	static bool createNodeList();
//
//	static bool addNode(tcplist* tcp_list);
//
//	static bool deleteNode(tcplist* p);
//
//	static struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);
//
//	static void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port, int status);
//
//	static void TCP_send(struct SockStruct_SocketLayer data_from_applayer);
//
//	static void TCP_receive(struct Msg_NetLayer data_from_netlayer);
//
//	static void TCP_resend();
//
//	static void TCP_close(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port);
//
//	void TCP_controller();
//
//	static void initialRTT();			//��ʼ��RTT
//
//	static float getSampleRTT(int sendtime, int gettime); //��̬���㳬ʱ���
//
//	static void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);
//
//	static void UDP_Send2IP(struct SockStruct_SocketLayer data_from_applayer, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len);
//
//	static bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seg_number);
//
//	static int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place);
//
//	static void port_listen(unsigned short port);
//
//	static bool check_listening(unsigned short port);
//
//	static void fill_new_tcplist(struct tcplist *new_tcp);
//
//	static void Temp_Send_ACK(struct tcplist *single_tcp);
//
//	static void time_over();
//
//};
//
//extern TRANSLAYER_API int nTransLayer;
//
//TRANSLAYER_API int fnTransLayer(void);
