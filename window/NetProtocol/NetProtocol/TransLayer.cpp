#pragma once
#include "stdafx.h"
#include "TransLayer.h"

tcplist* head = NULL;

int ACK_global;
int RTT;

static float EstimatedRTT;
static float DevRTT;
static int CountACK = 0;//冗余ACK计数器；
static int ACK_Now = -1;

bool createNodeList()
{
	head = (tcplist *)malloc(sizeof(struct tcplist));
	if (head == NULL)
	{
		return false;
	}
	else
	{
		head->next = NULL;
		head->tcp_src_ip = global_new_src_ip;
		head->tcp_dst_ip = global_new_dst_ip;
		head->tcp_src_port = global_new_src_port;
		head->tcp_dst_port = global_new_dst_port;
		head->cong_wind = MSS;
		head->threshold = INITIAL_THRESHOLD;
		head->seq_number = rand()%RANDOM_SEQ_NUMBER_MODULE;
		head->wait_for_ack = head->seq_number;
		head->wait_for_send = head->seq_number;
		head->wait_for_fill = head->seq_number;
		head->last_rcvd = 0;
		head->last_read = 0;
		head->rcvd_wind = INITIAL_RCVD_WIND;
		head->ack_count = 0;
		head->last_rcvd_ack = 0;
		head->next_send_ack = 0;
		return true;
	}
}

bool addNode(tcplist *tcp_list)
{
	if (NULL == head)
	{
		return false;
	}
	tcplist *p = head->next;
	tcplist *q = head;
	while (NULL != p)
	{
		q = p;
		p = p->next;
	}
	q->next = tcp_list;
	tcp_list->next = NULL;
	return true;
}

bool deleteNode(tcplist *p)
{
	tcplist *s1;
	tcplist *s2;
	s2 = s1 = head;

	if (s1 != p && s1 != NULL)
	{
		while (s1 != p && s1 != NULL)
		{
			s2 = s1;
			s1 = s1->next;
		}
	}
	else
	{
		printf("something wrong!");
		return false;
	}

	if (s1 == NULL)
	{
		printf("the node you want to delete doesn't exist!");
		return false;
	}
	else
	{
		if (s1 == head)
		{
			head = s1->next;
		}
		else if (s1->next == NULL)
		{
			s2->next = NULL;
		}
		else
		{
			s2->next = s1->next;
		}
		return true;
	}
}

struct tcplist *getNode(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{//在带头结点的单链表head中查找第i个结点，若找到（0≤i≤n），
	//则返回该结点的存储位置，否则返回NULL。
	tcplist *p;
	p = head;//从头结点开始扫描
	while (p)
	{//顺指针向后扫描，直到p->next为NULL为止
		if (p->tcp_src_ip == src_ip && p->tcp_src_port == src_port
			&& p->tcp_dst_ip == dst_ip && p->tcp_dst_port == dst_port)  //若找到目标IP，则返回p
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

bool global_TCP_new_flag;
unsigned int global_new_src_ip;
unsigned short global_new_src_port;
unsigned int global_new_dst_ip;
unsigned short global_new_dst_port;

bool global_TCP_send_flag;
struct sockstruct global_send_sockstruct;

bool global_TCP_receive_flag;
struct Msg global_receive_ip_msg;

bool global_TCP_resend_flag;

bool global_TCP_destroy_flag;
unsigned int global_destroy_src_ip;
unsigned short global_destroy_src_port;
unsigned int global_destroy_dst_ip;
unsigned short global_destroy_dst_port;

void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_new_flag);
	global_TCP_new_flag = true;
	global_new_src_ip = src_ip;
	global_new_src_port = src_port;
	global_new_dst_ip = dst_ip;
	global_new_dst_port = dst_port;
}

void TCP_send(struct sockstruct data_from_applayer)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_send_flag);
	global_TCP_send_flag = true;
	global_send_sockstruct = data_from_applayer;
}

void TCP_receive(struct Msg data_from_netlayer)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_receive_flag);
	global_TCP_receive_flag = true;
	global_receive_ip_msg = data_from_netlayer;
}

void TCP_resend()
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_resend_flag);
	global_TCP_resend_flag = true;
}

void TCP_destroy(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_destroy_flag);
	global_TCP_destroy_flag = true;
	global_destroy_src_ip = src_ip;
	global_destroy_src_port = src_port;
	global_destroy_dst_ip = dst_ip;
	global_destroy_dst_port = dst_port;
}



void TCP_controller()
{
	// 单线程总控的流程
	// 初始化随机器
	srand(time(0));
	// 初始化TCP连接链表
	createNodeList();
	// 初始化TCP操作标志
	global_TCP_new_flag = global_TCP_send_flag = global_TCP_resend_flag = global_TCP_receive_flag = global_TCP_destroy_flag = false;

	// 控制流
	for (;;)
	{
		// 是否需要新建一个TCP连接
		if (global_TCP_new_flag)
		{
			tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
			// 内存耗尽
			if (new_tcp == NULL)
			{
				printf("Out of memory! Cannot setup a new TCP link!\n");
				break;
			}
			
			new_tcp->next = NULL;
			new_tcp->tcp_src_ip = global_new_src_ip;
			new_tcp->tcp_dst_ip = global_new_dst_ip;
			new_tcp->tcp_src_port = global_new_src_port;
			new_tcp->tcp_dst_port = global_new_dst_port;
			new_tcp->cong_wind = MSS;
			new_tcp->threshold = INITIAL_THRESHOLD;
			new_tcp->seq_number = rand()%RANDOM_SEQ_NUMBER_MODULE;
			new_tcp->wait_for_ack = new_tcp->seq_number;
			new_tcp->wait_for_send = new_tcp->seq_number;
			new_tcp->wait_for_fill = new_tcp->seq_number;
			new_tcp->last_rcvd = 0;
			new_tcp->last_read = 0;
			new_tcp->rcvd_wind = INITIAL_RCVD_WIND;
			new_tcp->ack_count = 0;
			new_tcp->last_rcvd_ack = 0;
			new_tcp->next_send_ack = 0;
			addNode(new_tcp);
			global_TCP_new_flag = false;
		}

		// 是否需要发送一个TCP报文，这里并不是真正的发送，而是加入待发送报文队列
		// 等待合适时机向下递交
		if (global_TCP_send_flag)
		{
			// 处理数据
			unsigned int src_ip = getIP();
			unsigned short src_port = global_send_sockstruct.srcport;
			unsigned int dst_ip = IP_chars2uint(global_send_sockstruct.dstip);
			unsigned short dst_port = global_send_sockstruct.dstport;	

			// 根据四元组找到TCP连接链表中的对应表
			tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);
			
			// 发送缓冲区耗尽
			int unack_size = tcp->wait_for_fill-tcp->wait_for_ack;
			unack_size = unack_size < 0 ? unack_size+SEND_BUFFER_SIZE : unack_size;
			if (unack_size+global_send_sockstruct.datalength > SEND_BUFFER_SIZE)
			{
				printf("Out of send-buffer! Cannot send these data!\m");
				break;
			}

			// 将要发数据填入发送缓冲
			for (int i = 0; i < global_send_sockstruct.datalength; ++i)
			{
				tcp->tcp_buf_send[(i+tcp->wait_for_fill)%SEND_BUFFER_SIZE] = global_send_sockstruct.data[i];
			}
			// 更新待填充序号
			tcp->wait_for_fill = (tcp->wait_for_fill+global_send_sockstruct.datalength)%SEND_BUFFER_SIZE;
			global_TCP_send_flag = false;
		}

		// 是否需要接收一个TCP报文，将网络层交付的报文填入接收缓存，等待合适
		// 时机向上交付
		if (global_TCP_receive_flag)
		{
			// 处理数据
			unsigned int src_ip = global_receive_ip_msg.sip;
			unsigned short src_port = global_receive_ip_msg.ih_sport;
			unsigned int dst_ip = global_receive_ip_msg.dip;
			unsigned short dst_port = global_receive_ip_msg.ih_dport;

			// 根据四元组找到TCP连接链表中的对应表
			tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);
			
			// 接收缓冲区耗尽
			int unhandin_size = tcp->wait_for_fill-tcp->wait_for_ack;
			unhandin_size = unhandin_size < 0 ? unhandin_size+RCVD_BUFFER_SIZE : unhandin_size;
			if (unhandin_size+global_send_sockstruct.datalength > RCVD_BUFFER_SIZE)
			{
				printf("Out of receive-buffer! Cannot receive these data!\m");
				break;
			}

			// 将要发数据填入发送缓冲
			for (int i = 0; i < global_send_sockstruct.datalength; ++i)
			{
				tcp->tcp_buf_send[(i+tcp->wait_for_fill)%SEND_BUFFER_SIZE] = global_send_sockstruct.data[i];
			}
			// 更新待填充序号
			tcp->wait_for_fill = (tcp->wait_for_fill+global_send_sockstruct.datalength)%SEND_BUFFER_SIZE;
			global_TCP_send_flag = false;

//			// 更新对应TCP和Msg的window和ack
//			tcplist *temp1 = getNode(global_ip, global_port);
//			temp1->tcp_msg_rcvd[temp1->LastByteRcvd].tcpmessage = global_new_tcp_msg;
//			temp1->tcp_msg_rcvd[temp1->LastByteRcvd].datalen = global_datalen;
//			++(temp1->LastByteRcvd);
//			if (temp1->LastByteRcvd >= RCVD_BUFFER_SIZE)
//			{
//				temp1->LastByteRcvd = 0;
//			}
//			// 接收窗口更新
//			temp1->RcvWindow -= global_datalen;
//
//			// 计算RTT
//			RTT = (int)getSampleRTT(GetTickCount(), temp1->tcp_msg_send[temp1->MSG_ACK].time);
//
//			//是否有ACK
//			if (global_new_tcp_msg.tcp_ack != 0)
//			{
//				ACK_global = global_new_tcp_msg.tcp_ack_number;  //赋予ACK，当前ACK值
//				tcplist *temp2; 
//				temp2 = getNode(global_ip, global_port);
//
//				// 冗余ack，计数
//				if (temp1->last_ACK == global_new_tcp_msg.tcp_ack_number)
//				{
//					++(temp1->ACK_count);
//				}
//				// 非冗余ack，确认
//				else
//				{
//					temp1->last_ACK = global_new_tcp_msg.tcp_ack_number;
//				}
//
//				//if (temp2->tcp_msg_send[temp2->MSG_ACK].tcpmessage.tcp_seq_number >= ACK_global)   //冗余ACK计数
//				//{
//				//	temp2->tcp_msg_send[temp2->MSG_ACK].ACK++;  
//				//}
//				//else
//				//{
//				//	temp2->MSG_ACK++;      //当前待确认的报文得到确认了，下标自增，指向下一个正待确认的已经发送的报文
//				//}
//
//				if (temp2->cwnd <= temp2->Threshold) //慢启动
//				{
//					temp2->cwnd += MSS;
//				}
//				else
//				{
//					//if (temp2->tcp_msg_send[temp2->MSG_ACK].ACK >= 3)    //收到3个冗余ACK，设置为拥塞避免
//					//{
//					//	temp2->Threshold = temp2->cwnd / 2;
//					//	temp2->cwnd = temp2->Threshold;
//					//	//sendtoip(temp3->tcp_msg_send[temp2->MSG_ACK].tcpmessage, temp2->IP, 第一个参数这个报文的长度);
//					//}
//					//else      //收到前面未确认数据的ACK
//					//{
//					//	temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
//					//}
//				}
//				ACK_global = 0;
//
//
//			}
//
//			global_TCP_send_flag = false;
//
//		}
//
//		// 重传..有点问题？
//		if (global_TCP_resend_flag)
//		{
//		}
//
//		// 是否需要拆除一个TCP连接
//		if (global_TCP_destroy_flag)
//		{
//			// 拆除TCP连接
//			deletenode(getNode(global_ip, global_port));
//			global_TCP_destroy_flag = false;
//		}
//
// 
//
//
//
//		tcplist* temp3 = head;
//		while (temp3)         //实时检查每个TCP下当前正待响应的报文是否超时未响应
//		{
//			if (GetTickCount() - temp3->tcp_msg_send[temp3->MSG_ACK].time > RTT)
//			{
//				temp3->Threshold = temp3->cwnd / 2;
//				temp3->cwnd = MSS;
//				temp3->tcp_msg_send[temp3->MSG_ACK].time = GetTickCount();
//				//sendtoip(temp3->tcp_msg_send[temp3->MSG_ACK].tcpmessage, temp3->IP, 第一个参数这个报文的长度);
//			}
//			temp3 = temp3->next;
//		}
//		//
//
//
//
	}
}
//
//int Count_ACK(int ACK_global)         //收到3次冗余ACK返回global_TCP_resend_flag的值
//{
//	if (ACK_Now != ACK_global) //New ID
//	{
//		CountACK = 1;
//		ACK_Now = ACK_global;
//		return 0;
//	}
//	else
//	{
//		if (CountACK == 2)
//			return 1;
//		else
//		{
//			CountACK += 1;
//			return 0;
//		}
//	}
//}
//
//int Fastretransmit(int ACK_global)    //返回需要重发的ACK序号
//{
//	int retransmitACKID;
//	retransmitACKID = ACK_global;
//	return retransmitACKID;
//}
//
//int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum) //返回需要重发的ACK序号,-1表示不需要重传
//{
//	int sum;
//	sum = tcpmakesum(len_tcp, src_port, dest_port, padding, buff) + checksum;
//	if (sum != 0xffff)                                                                                    //发生错误
//	{
//		return ACK_global;
//	}
//	else
//	{
//		return -1;
//	}
//}
//
//void initialRTT()                   //初始化RTT需要的变量
//{
//	EstimatedRTT = 0;
//	DevRTT = 0;
//}
//
//float getSampleRTT(int sendtime, int gettime)		//往返时延的估计与超时，返回超时时间
//{
//	float alpha = 0.125;
//	float beta = 0.25;
//	float TimeoutInterval;
//	int sampleRTT;
//	sampleRTT = gettime - sendtime;
//	EstimatedRTT = (1 - alpha)*EstimatedRTT + alpha*sampleRTT;
//	DevRTT = (1 - beta)*DevRTT + beta*abs(sampleRTT - EstimatedRTT);
//	TimeoutInterval = EstimatedRTT + 4 * DevRTT;
//	return TimeoutInterval;
//	//printf("sampleRTT = %d \n", sampleRTT);
//	//printf("test = %f\n", 0.125*sampleRTT);
//	//printf("test2 = %f \n", (1 - alpha)*EstimatedRTT);
//	//printf("EstimatedRTT = %f \n", EstimatedRTT);
//	//printf("DevRTT = %f \n", DevRTT);
//}
//
//void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int dst_ip, unsigned int data_len)
//{
//	struct Msg new_ip_msg;
//	new_ip_msg.sip = getIP();
//	new_ip_msg.dip = dst_ip;
//	new_ip_msg.ih_sport = send_tcp_message.tcp_src_port;
//	new_ip_msg.ih_dport = send_tcp_message.tcp_dst_port;
//	new_ip_msg.datelen = data_len+send_tcp_message.tcp_hdr_length;
//	memcpy(new_ip_msg.data, &send_tcp_message, new_ip_msg.datelen);
//	new_ip_msg.protocol = PROTOCOL_TCP;	// 6 for TCP
//
//	// 发往网络层
//	AfxGetApp()->m_pMainWnd->SendMessage(IPTOLINK, (WPARAM)&new_ip_msg, (LPARAM)0);	// 第三个参数完全不需要
//}