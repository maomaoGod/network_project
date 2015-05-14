#pragma once
#include "stdafx.h"
#include "TransLayer.h"

tcplist* head = NULL;

int ACK_global;
int RTT;

static float EstimatedRTT;
static float DevRTT;
static int CountACK = 0;//����ACK��������
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
		head->send_ack_needed = false;
		head->wait_for_ack_msg = 0;
		head->wait_for_fill_msg = 0;
		head->last_rcvd_msg = 0;
		head->last_read_msg = 0;
		head->status = CONG_SS;
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
{//�ڴ�ͷ���ĵ�����head�в��ҵ�i����㣬���ҵ���0��i��n����
	//�򷵻ظý��Ĵ洢λ�ã����򷵻�NULL��
	tcplist *p;
	p = head;//��ͷ��㿪ʼɨ��
	while (p)
	{//˳ָ�����ɨ�裬ֱ��p->nextΪNULLΪֹ
		if (p->tcp_src_ip == src_ip && p->tcp_src_port == src_port
			&& p->tcp_dst_ip == dst_ip && p->tcp_dst_port == dst_port)  //���ҵ�Ŀ��IP���򷵻�p
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
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_new_flag);
	global_TCP_new_flag = true;
	global_new_src_ip = src_ip;
	global_new_src_port = src_port;
	global_new_dst_ip = dst_ip;
	global_new_dst_port = dst_port;
}

void TCP_send(struct sockstruct data_from_applayer)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_send_flag);
	global_TCP_send_flag = true;
	global_send_sockstruct = data_from_applayer;
}

void TCP_receive(struct Msg data_from_netlayer)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_receive_flag);
	global_TCP_receive_flag = true;
	global_receive_ip_msg = data_from_netlayer;
}

void TCP_resend()
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_resend_flag);
	global_TCP_resend_flag = true;
}

void TCP_destroy(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_destroy_flag);
	global_TCP_destroy_flag = true;
	global_destroy_src_ip = src_ip;
	global_destroy_src_port = src_port;
	global_destroy_dst_ip = dst_ip;
	global_destroy_dst_port = dst_port;
}



void TCP_controller()
{
	// ���߳��ܿص�����
	// ��ʼ�������
	srand(time(0));
	// ��ʼ��TCP��������
	createNodeList();
	// ��ʼ��TCP������־
	global_TCP_new_flag = global_TCP_send_flag = global_TCP_resend_flag = global_TCP_receive_flag = global_TCP_destroy_flag = false;

	// ������
	for (;;)
	{
		// �Ƿ���Ҫ�½�һ��TCP����
		if (global_TCP_new_flag)
		{
			tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
			// �ڴ�ľ�
			if (new_tcp == NULL)
			{
				printf("Out of memory! Cannot setup a new TCP link!\n");
				goto ctrl_send;
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
			new_tcp->send_ack_needed = false;
			new_tcp->wait_for_ack_msg = 0;
			new_tcp->wait_for_fill_msg = 0;
			new_tcp->last_rcvd_msg = 0;
			new_tcp->last_read_msg = 0;
			new_tcp->status = CONG_SS;
			addNode(new_tcp);
			global_TCP_new_flag = false;
		}

		// �Ƿ���Ҫ����һ��TCP���ģ����ﲢ���������ķ��ͣ����Ǽ�������ͱ��Ķ���
		// �ȴ�����ʱ�����µݽ�
ctrl_send:
		if (global_TCP_send_flag)
		{
			// ��������
			unsigned int src_ip = getIP();
			unsigned short src_port = global_send_sockstruct.srcport;
			unsigned int dst_ip = IP_chars2uint(global_send_sockstruct.dstip);
			unsigned short dst_port = global_send_sockstruct.dstport;	

			// ������Ԫ���ҵ�TCP���������еĶ�Ӧ��
			tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);
			
			// ���ͻ������ľ�
			int unack_size = tcp->wait_for_fill-tcp->wait_for_ack;
			if (unack_size+global_send_sockstruct.datalength > SEND_BUFFER_SIZE)
			{
				printf("Out of send-buffer! Cannot send these data!\m");
				goto ctrl_receive;
			}

			// ��Ҫ���������뷢�ͻ���
			for (int i = 0; i < global_send_sockstruct.datalength; ++i)
			{
				tcp->tcp_buf_send[(i+tcp->wait_for_fill)%SEND_BUFFER_SIZE] = global_send_sockstruct.data[i];
			}
			// ���´�������
			tcp->wait_for_fill = tcp->wait_for_fill+global_send_sockstruct.datalength;
			global_TCP_send_flag = false;
		}

		// �Ƿ���Ҫ����һ��TCP���ģ�������㽻���ı���������ջ��棬�ȴ�����
		// ʱ�����Ͻ���
ctrl_receive:
		if (global_TCP_receive_flag)
		{
			// ��ȡTCP���Ķ�
			struct tcp_message new_tcp_msg;
			memcpy(&new_tcp_msg, global_receive_ip_msg.data, global_receive_ip_msg.datelen);

			// opts��dataһͬ���м���
			unsigned opts_data_len = global_receive_ip_msg.datelen-20;

			// �����
			if (!udpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			{
				// ��������
				goto ctrl_destroy;
			}
			
			// ��������
			unsigned int src_ip = global_receive_ip_msg.sip;
			unsigned short src_port = global_receive_ip_msg.ih_sport;
			unsigned int dst_ip = global_receive_ip_msg.dip;
			unsigned short dst_port = global_receive_ip_msg.ih_dport;

			// ������Ԫ���ҵ�TCP���������еĶ�Ӧ��
			tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);
			
			int data_len = global_receive_ip_msg.datelen-new_tcp_msg.tcp_hdr_length;
			int opts_offset = new_tcp_msg.tcp_hdr_length-20;
			if (tcp->last_rcvd > new_tcp_msg.tcp_seq_number)
			{
				// ������֮ǰδ�յ��ı��ģ��������ظ��յ��ı���
				if (rcvd_msg_existed(tcp, new_tcp_msg.tcp_seq_number))
				{
					// ���ظ��յ��ı��ģ�����
					goto ctrl_destroy;
				}
				else
				{
					// ��֮ǰδ�յ��ı���
					if (tcp->last_read+1 == new_tcp_msg.tcp_seq_number)
					{
						// ǡ��������֮ǰ�ȴ��ģ�����
						// ������������������ջ��壬������opts
						for (int i = 0; i < data_len; ++i)
						{
							tcp->tcp_buf_rcvd[(i+tcp->last_read+1)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
						}
						// ����������������һ���ֽ��������

						// ������ձ��Ķ���
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = tcp->last_read+1;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;
					}
					else
					{
						// ��������֮ǰ�ȴ��ģ�����Ҳ�����϶
						// ������������������ջ��壬������opts
						for (int i = 0; i < data_len; ++i)
						{
							tcp->tcp_buf_rcvd[(i+new_tcp_msg.tcp_seq_number)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
						}
						// ����������������һ���ֽ��������

						// ������ձ��Ķ���
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = new_tcp_msg.tcp_seq_number;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;
					}
				}
			}
			else
			{
				// �����������ڴ��ı��ģ�Ҳ�������������ģ���Ҫ��������ܽ���
				if (tcp->last_rcvd+1 == new_tcp_msg.tcp_seq_number)
				{
					// ���ջ������ľ��������������ƣ�һ�㲻���ܷ�����
					int unhandin_size = tcp->last_rcvd-tcp->last_read;
					if (unhandin_size+global_receive_ip_msg.datelen > RCVD_BUFFER_SIZE)
					{
						printf("Out of receive-buffer! Cannot store these data!\m");
						goto ctrl_destroy;
					}

					// ǡ��������֮ǰ�ȴ��ģ�����
					// ������������������ջ��壬������opts
					for (int i = 0; i < data_len; ++i)
					{
						tcp->tcp_buf_rcvd[(i+tcp->last_rcvd+1)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
					}
					// ��������������һ���ֽ��������
					tcp->last_rcvd = tcp->last_rcvd+data_len;

					// ������ձ��Ķ���
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = tcp->last_rcvd+1;
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;
				}
				else
				{
					// ���ջ������ľ��������������ƣ�һ�㲻���ܷ�����
					int unhandin_size = new_tcp_msg.tcp_seq_number-1-tcp->last_read;
					if (unhandin_size+global_receive_ip_msg.datelen > RCVD_BUFFER_SIZE)
					{
						printf("Out of receive-buffer! Cannot store these data!\m");
						goto ctrl_destroy;
					}

					// ��������֮ǰ�ȴ��ģ�����������
					// ������������������ջ��壬������opts
					for (int i = 0; i < data_len; ++i)
					{
						tcp->tcp_buf_rcvd[(i+new_tcp_msg.tcp_seq_number)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
					}
					// ��������������һ���ֽ��������
					tcp->last_rcvd = new_tcp_msg.tcp_seq_number+data_len;

					// ������ձ��Ķ���
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = new_tcp_msg.tcp_seq_number;
					tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;
				}
			}

			// ���¶Է��Ľ��մ���
			tcp->rcvd_wind = new_tcp_msg.tcp_rcv_window;

			// ack�ֶ��Ƿ���Ч
			if (new_tcp_msg.tcp_ack != 0)
			{
				// ack��Ч���ж��Ƿ��������ȷ��ack
				if (new_tcp_msg.tcp_ack_number >= tcp->wait_for_ack)
				{
					// �ۼ�ȷ��ack
					tcp->wait_for_ack = new_tcp_msg.tcp_ack_number+data_len;
					// �������ȷ�ϣ���ֹ�ô�ack�����϶
					tcp->wait_for_ack = next_ack_place(tcp, tcp->wait_for_ack);
				}
				else
				{
					// ����ack
					if (new_tcp_msg.tcp_ack_number == tcp->last_rcvd_ack)
					{
						++(tcp->ack_count);
						if (tcp->ack_count >= 3)
						{
							// 3������ack�������ش�
							// fast_re_send
						}
					}
					else
					{
						tcp->last_rcvd_ack = new_tcp_msg.tcp_ack_number;
						tcp->ack_count = 0;
					}
				}
			}

			// ��ʲô�ã�
			// ����RTT
			RTT = (int)getSampleRTT(GetTickCount(), tcp->tcp_msg_send[tcp->wait_for_ack_msg].time);

			global_TCP_send_flag = false;

		}

		// �ش�..�е����⣿
		if (global_TCP_resend_flag)
		{
		}

		// �Ƿ���Ҫ���һ��TCP����
ctrl_destroy:
		if (global_TCP_destroy_flag)
		{
			// ���TCP����
			deleteNode(getNode(global_destroy_src_ip, global_destroy_src_port, global_destroy_dst_ip, global_destroy_dst_port));
			global_TCP_destroy_flag = false;
		}

 



		tcplist* temp3 = head;
		while (temp3)         //ʵʱ���ÿ��TCP�µ�ǰ������Ӧ�ı����Ƿ�ʱδ��Ӧ
		{
			if (GetTickCount() - temp3->tcp_msg_send[temp3->wait_for_ack_msg].time > RTT)
			{
				temp3->Threshold = temp3->cwnd / 2;
				temp3->cwnd = MSS;
				temp3->tcp_msg_send[temp3->wait_for_ack_msg].time = GetTickCount();
				//sendtoip(temp3->tcp_msg_send[temp3->MSG_ACK].tcpmessage, temp3->IP, ��һ������������ĵĳ���);
			}
			temp3 = temp3->next;
		}
		//

		tcplist* temp3 = head;
		while (temp3)
		{
			//���ͱ���
			while (min(temp3->wait_for_fill, temp3->wait_for_send + MSS) - temp3->wait_for_ack <= min(temp3->cwnd, temp3->RcvWindow))
			{
				int new_send;
		//		new_send = min(temp3->wait_for_ack + min(temp3->cwnd, temp3->RcvWindow), min(temp3->wait_for_fill, temp3->wait_for_send + MSS));
				new_send = min(temp3->wait_for_fill, temp3->wait_for_send + MSS);
				temp3->tcp_msg_send[temp3->wait_for_fill_msg].datalen = new_send - temp3->wait_for_send;
				temp3->tcp_msg_send[temp3->wait_for_fill_msg].time = GetTickCount();
				temp3->tcp_msg_send[temp3->wait_for_fill_msg].seq_number = temp3->wait_for_send;
				tcp_message temp4;
				temp4.tcp_src_port = temp3->tcp_src_port;
				temp4.tcp_dst_port = temp3->tcp_dst_port;
				temp4.tcp_hdr_length = 20;
				memcpy(temp4.tcp_opts_and_app_data, &temp3->tcp_buf_send[temp3->wait_for_send], temp3->tcp_msg_send[temp3->wait_for_fill_msg].datalen);
				TCP_Send2IP(temp4, temp3->tcp_dst_ip, temp3->tcp_msg_send[temp3->wait_for_fill_msg].datalen);
				temp3->wait_for_send = new_send;
				temp3->wait_for_fill_msg++;
			}
			temp3 = temp3->next;
		}
		free(temp3);



	}
}

int Count_ACK(int ACK_global)         //�յ�3������ACK����global_TCP_resend_flag��ֵ
{
	if (ACK_Now != ACK_global) //New ID
	{
		CountACK = 1;
		ACK_Now = ACK_global;
		return 0;
	}
	else
	{
		if (CountACK == 2)
			return 1;
		else
		{
			CountACK += 1;
			return 0;
		}
	}
}

int Fastretransmit(int ACK_global)    //������Ҫ�ط���ACK���
{
	int retransmitACKID;
	retransmitACKID = ACK_global;
	return retransmitACKID;
}

int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum) //������Ҫ�ط���ACK���,-1��ʾ����Ҫ�ش�
{
	int sum;
	sum = tcpmakesum(len_tcp, src_port, dest_port, padding, buff) + checksum;
	if (sum != 0xffff)                                                                                    //��������
	{
		return ACK_global;
	}
	else
	{
		return -1;
	}
}

void initialRTT()                   //��ʼ��RTT��Ҫ�ı���
{
	EstimatedRTT = 0;
	DevRTT = 0;
}

float getSampleRTT(int sendtime, int gettime)		//����ʱ�ӵĹ����볬ʱ�����س�ʱʱ��
{
	float alpha = 0.125;
	float beta = 0.25;
	float TimeoutInterval;
	int sampleRTT;
	sampleRTT = gettime - sendtime;
	EstimatedRTT = (1 - alpha)*EstimatedRTT + alpha*sampleRTT;
	DevRTT = (1 - beta)*DevRTT + beta*abs(sampleRTT - EstimatedRTT);
	TimeoutInterval = EstimatedRTT + 4 * DevRTT;
	return TimeoutInterval;
	//printf("sampleRTT = %d \n", sampleRTT);
	//printf("test = %f\n", 0.125*sampleRTT);
	//printf("test2 = %f \n", (1 - alpha)*EstimatedRTT);
	//printf("EstimatedRTT = %f \n", EstimatedRTT);
	//printf("DevRTT = %f \n", DevRTT);
}

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int dst_ip, unsigned int data_len)
{
	struct Msg new_ip_msg;
	new_ip_msg.sip = getIP();
	new_ip_msg.dip = dst_ip;
	new_ip_msg.ih_sport = send_tcp_message.tcp_src_port;
	new_ip_msg.ih_dport = send_tcp_message.tcp_dst_port;
	new_ip_msg.datelen = data_len+send_tcp_message.tcp_hdr_length;
	memcpy(new_ip_msg.data, &send_tcp_message, new_ip_msg.datelen);
	new_ip_msg.protocol = PROTOCOL_TCP;	// 6 for TCP

	// ���������
	AfxGetApp()->m_pMainWnd->SendMessage(IPTOLINK, (WPARAM)&new_ip_msg, (LPARAM)0);	// ������������ȫ����Ҫ
}

bool rcvd_msg_existed(struct tcplist *tcp, unsigned int seq_number)
{
	if (seq_number <= tcp->last_read)
	{
		return true;
	}
	for (int i = tcp->last_read_msg+1; i <= tcp->last_rcvd_msg; ++i)
	{
		if (tcp->tcp_msg_rcvd[i%RCVD_STRUCT_SIZE].seq_number == seq_number)
		{
			return true;
		}
	}
	return false;
}

int next_ack_place(struct tcplist *tcp, unsigned int init_ack_place)
{
	int next_ack = init_ack_place;
	for (int i = tcp->wait_for_ack_msg; i < tcp->wait_for_fill_msg; ++i)
	{
		if (tcp->tcp_msg_send[i%SEND_STRUCT_SIZE].seq_number == next_ack)
		{
			next_ack += tcp->tcp_msg_send[i%SEND_STRUCT_SIZE].datalen;
			i = tcp->wait_for_ack_msg;
		}
	}
	return next_ack;
}