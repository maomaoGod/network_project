#pragma once
#include "stdafx.h"
#include "TransLayer.h"

struct tcplist* head = NULL;

int ACK_global;	// ???
int RTT;		// ???

static float EstimatedRTT;
static float DevRTT;
static int CountACK = 0;	//����ACK��������???
static int ACK_Now = -1;	// ???

bool global_TCP_new_flag;
unsigned int global_new_src_ip;
unsigned short global_new_src_port;
unsigned int global_new_dst_ip;
unsigned short global_new_dst_port;
int global_new_status;

bool global_TCP_send_flag;
struct tcplist *global_send_tcp;
struct sockstruct global_send_sockstruct;

bool global_TCP_receive_flag;
struct Msg global_receive_ip_msg;

bool global_TCP_resend_flag;

bool global_TCP_close_flag;
struct tcplist *global_close_tcp;

struct stopandwait new_sandw;

bool global_sandw_send_flag;
struct sockstruct global_send_sockstruct_sandw;

bool global_sandw_receive_flag;
struct Msg global_receive_ip_msg_sandw;

bool global_sandw_sendtoIP_flag;

bool global_SR_send_flag;		//SR�����ź�
bool global_SR_receive_flag;	//SR�����ź�
bool global_SR_send2IP_flag;	//SRĿ��IP�����ź�

struct sockstruct global_send_sockstruct_SR;
struct Msg global_receive_ip_msg_SR;

bool listening_flag[MAX_PORT];

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
		head->cong_status = CONG_SS;
		head->tcp_established_syn_seq = -1;
		head->connect_status = -1;
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

void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port, int status)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_new_flag);
	global_new_src_ip = src_ip;
	global_new_src_port = src_port;
	global_new_dst_ip = dst_ip;
	global_new_dst_port = dst_port;
	global_new_status = status;
	global_TCP_new_flag = true;
	while (global_TCP_new_flag);
}

void TCP_send(struct sockstruct data_from_applayer)
{
	struct tcplist *found_tcp = getNode(getIP(), data_from_applayer.srcport,
										IP_chars2uint(data_from_applayer.dstip), data_from_applayer.dstport);
	if (found_tcp == NULL)
	{
		// δ����TCP����
		printf("TCP link has not been established!\n");
		return;
	}
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_send_flag);
	global_send_tcp = found_tcp;
	global_send_sockstruct = data_from_applayer;
	global_TCP_send_flag = true;
	while (global_TCP_send_flag);
}

void TCP_receive(struct Msg data_from_netlayer)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_receive_flag);
	global_receive_ip_msg = data_from_netlayer;
	global_TCP_receive_flag = true;
	while (global_TCP_receive_flag);
}

void TCP_resend()
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_resend_flag);
	global_TCP_resend_flag = true;
	while (global_TCP_resend_flag);
}

void TCP_close(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{
	struct tcplist *found_tcp = getNode(src_ip, src_port, dst_ip, dst_port);
	if (found_tcp == NULL)
	{
		// δ����TCP���ӣ���Ĭ
		printf("TCP link has not been established!\n");
		return;
	}
	while (global_TCP_close_flag);
	global_close_tcp = found_tcp;
	global_TCP_close_flag = true;
	while (global_TCP_close_flag);
}

void TCP_controller()
{
	// ���߳��ܿص�����
	// ��ʼ�������
	srand(time(0));
	// ��ʼ��TCP��������
	createNodeList();
	// ��ʼ��TCP������־
	global_TCP_new_flag = global_TCP_send_flag = global_TCP_resend_flag = global_TCP_receive_flag = global_TCP_close_flag = false;
	// ��ʼ��������־
	memset(listening_flag, 0, sizeof(listening_flag));

	// ������
	for (;;)
	{
		// �Ƿ���Ҫ�½�һ��TCP����
		if (global_TCP_new_flag)
		{
			struct tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
			// �ڴ�ľ�
			if (new_tcp == NULL)
			{
				printf("Out of memory! Cannot setup a new TCP link!\n");
				global_TCP_new_flag = false;
				goto ctrl_send;
			}

			new_tcp->tcp_src_ip = global_new_src_ip;
			new_tcp->tcp_dst_ip = global_new_dst_ip;
			new_tcp->tcp_src_port = global_new_src_port;
			new_tcp->tcp_dst_port = global_new_dst_port;
			new_tcp->connect_status = global_new_status;
			fill_new_tcplist(new_tcp);
			addNode(new_tcp);
			global_TCP_new_flag = false;
		}

		// �Ƿ���Ҫ����һ��TCP���ģ����ﲢ���������ķ��ͣ����Ǽ�������ͱ��Ķ���
		// �ȴ�����ʱ�����µݽ�
ctrl_send:
		if (global_TCP_send_flag)
		{
			// ��������
			//unsigned int src_ip = getIP();
			//unsigned short src_port = global_send_sockstruct.srcport;
			//unsigned int dst_ip = IP_chars2uint(global_send_sockstruct.dstip);
			//unsigned short dst_port = global_send_sockstruct.dstport;	

			// ������Ԫ���ҵ�TCP���������еĶ�Ӧ��
			struct tcplist *tcp = global_send_tcp;
			
			// ���ͻ������ľ�
			int unack_size = tcp->wait_for_fill-tcp->wait_for_ack;
			if (unack_size+global_send_sockstruct.datalength > SEND_BUFFER_SIZE)
			{
				printf("Out of send-buffer! Cannot send these data!\n");
				global_TCP_send_flag = false;
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

			////////// �����
			////////if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			////////{
			////////	// ��������
			////////	global_TCP_receive_flag = false;
			////////	goto ctrl_close;
			////////}
			
			// �������ݣ���Ҫ������������
			unsigned int dst_ip = global_receive_ip_msg.sip;
			unsigned short dst_port = new_tcp_msg.tcp_src_port;
			unsigned int src_ip = global_receive_ip_msg.dip;
			unsigned short src_port = new_tcp_msg.tcp_dst_port;

			// ������Ԫ���ҵ�TCP���������еĶ�Ӧ��
			struct tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);

			// ����һ������
			if (new_tcp_msg.tcp_syn == 1 && new_tcp_msg.tcp_ack == 0)
			{
				if (!check_listening(src_port))
				{
					// δ��������Ĭ����
					global_TCP_receive_flag = false;
					goto ctrl_close;
				}

				struct tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
				// �ڴ�ľ�
				if (new_tcp == NULL)
				{
					printf("Out of memory! Cannot setup a new TCP link!\n");
					global_TCP_receive_flag = false;
					goto ctrl_close;
				}

				new_tcp->tcp_src_ip = src_ip;
				new_tcp->tcp_dst_ip = dst_ip;
				new_tcp->tcp_src_port = src_port;
				new_tcp->tcp_dst_port = dst_port;
				new_tcp->connect_status = LINK_CONNECTED;
				fill_new_tcplist(new_tcp);
				addNode(new_tcp);

				// ���¶Է���seq_numberϵ��
				new_tcp->send_ack_needed = true;
				new_tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
				new_tcp->last_read = new_tcp->last_rcvd = new_tcp_msg.tcp_seq_number;
			}

			// ���ڶ�������
			else if (new_tcp_msg.tcp_syn == 1 && new_tcp_msg.tcp_ack == 1)
			{
				if (tcp == NULL)
				{
					// α��ĵڶ�������
					printf("Under attack of fake shaking message!\n");
					global_TCP_receive_flag = false;
					goto ctrl_close;
				}
				else
				{
					// ȷʵ�ڵȴ��ڶ�������
					if (tcp->connect_status == LINK_WAIT_FOR_SYN)
					{
						tcp->connect_status = LINK_GOT_SYN;

						// ���¶Է���seq_numberϵ��
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						tcp->last_read = tcp->last_rcvd = new_tcp_msg.tcp_seq_number;
					}
					// ���ڵȴ��ڶ������֣���Ĭ������������������
					else
					{
						// do nothing
					}
				}
			}

			// ������������
			else if (tcp != NULL && tcp->connect_status == LINK_WAIT_FOR_SYNACK && new_tcp_msg.tcp_ack == 1)
			{
				// ȷʵ�ڵȴ�����������
				tcp->connect_status = LINK_GOT_SYNACK;

				// ���¶Է���seq_numberϵ��
				tcp->send_ack_needed = false;
				++(tcp->last_read);
				++(tcp->last_rcvd);
			}

			// �յ�FIN
			else if (new_tcp_msg.tcp_fin == 1 && new_tcp_msg.tcp_ack == 0)
			{
				if (tcp == NULL)
				{
					// α��ĶϿ����ӣ���Ĭ����
				}
				else
				{
					// ȷʵ���Ա�FIN������δ�뿪
					if (tcp->connect_status == LINK_CONNECT_BIDIR)
					{
						// ״̬��Ϊ�����뿪
						tcp->connect_status = LINK_FINISHED;

						// ���¶Է���seq_numberϵ��
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						// ���������δȷ�ϾͲ����Է���FIN�����Կ���ֱ�ӱ�ʾread����
						++(tcp->last_read);
						++(tcp->last_rcvd);
					}
					// ȷʵ�ڿ���FIN�������Ѱ뿪
					else if (tcp->connect_status == LINK_SELF_HALF_OPEN)
					{
						// ״̬��Ϊ���Ӷ�ʧ
						tcp->connect_status = LINK_CONNECT_DESTROYED;

						// ���¶Է���seq_numberϵ��
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						// ���������δȷ�ϾͲ����Է���FIN�����Կ���ֱ�ӱ�ʾread����
						++(tcp->last_read);
						++(tcp->last_rcvd);
					}
					// ���ڵȴ�FINACK����Ĭ������������������
					else
					{
						// do nothing
					}
				}
			}

			// �յ�FINACK
			else if (new_tcp_msg.tcp_fin == 1 && new_tcp_msg.tcp_ack == 1)
			{
				if (tcp == NULL)
				{
					// α��ĶϿ����ӣ���Ĭ����
				}
				else
				{
					// ȷʵ�ڵȴ�FINACK���Է�δ�뿪
					if (tcp->connect_status == LINK_WAIT_FOR_FINACK)
					{
						// ״̬��Ϊ�����뿪
						tcp->connect_status = LINK_SELF_HALF_OPEN;
					}
					// ȷʵ�ڵȴ�FINACK���Է��Ѱ뿪
					else if (tcp->connect_status == LINK_WAIT_FOR_DESACK)
					{
						// ״̬��Ϊ���Ӷ�ʧ
						tcp->connect_status = LINK_CONNECT_LOSE;
					}
					// ���ڵȴ�FINACK����Ĭ������������������
					else
					{
						// do nothing
					}
				}
			}
			
			// ����TCP������Ϣ����һ�㱨��
			else
			{
				int data_len = global_receive_ip_msg.datelen-4*new_tcp_msg.tcp_hdr_length;
				int opts_offset = 4*new_tcp_msg.tcp_hdr_length-20;

				// Temp----------------------------------------------------------------
				if (data_len == 0)
				{
					goto ctrl_ack;
				}
				// Temp----------------------------------------------------------------

				if (tcp->last_rcvd > new_tcp_msg.tcp_seq_number)
				{
					// ������֮ǰδ�յ��ı��ģ��������ظ��յ��ı���
					if (rcvd_msg_existed(tcp, new_tcp_msg.tcp_seq_number))
					{
						// ���ظ��յ��ı��ģ�����
						global_TCP_receive_flag = false;
						goto ctrl_close;
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

							// ����ack
							tcp->send_ack_needed = true;
							tcp->next_send_ack = new_tcp_msg.tcp_seq_number+data_len;
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

							// ����ack
							tcp->send_ack_needed = true;
							tcp->next_send_ack = tcp->last_read+1;
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
							printf("Out of receive-buffer! Cannot store these data!\n");
							global_TCP_receive_flag = false;
							goto ctrl_close;
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

						// ����ack
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number + data_len;
					}
					else
					{
						// ���ջ������ľ��������������ƣ�һ�㲻���ܷ�����
						int unhandin_size = new_tcp_msg.tcp_seq_number-1-tcp->last_read;
						if (unhandin_size+global_receive_ip_msg.datelen > RCVD_BUFFER_SIZE)
						{
							printf("Out of receive-buffer! Cannot store these data!\n");
							global_TCP_receive_flag = false;
							goto ctrl_close;
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

						// ����ack
						tcp->send_ack_needed = true;
						tcp->next_send_ack = tcp->last_read+1;
					}
				}

				// ���¶Է��Ľ��մ���
				tcp->rcvd_wind = new_tcp_msg.tcp_rcv_window;

ctrl_ack:
				// ack�ֶ��Ƿ���Ч
				if (new_tcp_msg.tcp_ack != 0)
				{
					if (tcp->cong_status == CONG_SS)
					{
						tcp->cong_wind = tcp->cong_wind + MSS;
						if (tcp->cong_wind > tcp->threshold)
						{
							tcp->cong_status = CONG_CA;
						}
					}
					else if (tcp->cong_status == CONG_CA)
					{
						tcp->cong_wind = tcp->cong_wind + MSS*MSS / tcp->cong_wind;
					}
					// ack��Ч���ж��Ƿ��������ȷ��ack
					if (new_tcp_msg.tcp_ack_number >= tcp->wait_for_ack)
					{
						// �ۼ�ȷ��ack
						tcp->wait_for_ack = new_tcp_msg.tcp_ack_number;
						//// �������ȷ�ϣ���ֹ�ô�ack�����϶�����ӻ����շ���
						//tcp->wait_for_ack = next_ack_place(tcp, tcp->wait_for_ack);
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
								tcp->threshold = tcp->cong_wind / 2;
								tcp->cong_wind = tcp->threshold;
							}
						}
						else
						{
							tcp->last_rcvd_ack = new_tcp_msg.tcp_ack_number;
							tcp->ack_count = 0;
						}
					}
				}

				// ����ACK���Ժ���
				//Temp_Send_ACK(tcp); Dead Lock!!!!
			}

			// ��ʲô�ã�ERROR....EDITING Needed
			// ����RTT
			//RTT = (int)getSampleRTT(tcp->tcp_msg_send[tcp->wait_for_ack_msg].time, GetTickCount());

			global_TCP_receive_flag = false;
		}

		// �ش�..�е����⣿
		if (global_TCP_resend_flag)
		{
		}

		// �ر�TCP���ӣ����˫�ر�����
ctrl_close:
		if (global_TCP_close_flag)
		{
			// ˫�����رվͲ��TCP����
			if (global_close_tcp->connect_status = LINK_PEER_HALF_OPEN)
			{
				// �Է��Ѿ��ر�
				global_close_tcp->connect_status = LINK_CONNECT_DESTROYING;
			}
			else
			{
				// �رձ�����tcp���ӣ�������Ȼ���Խ��յ��Է������ݣ����Է�ack
				global_close_tcp->connect_status = LINK_FINISHING;
			}
		}

		//tcplist* single_tcp = head;
		//while (single_tcp)         //ʵʱ���ÿ��TCP�µ�ǰ������Ӧ�ı����Ƿ�ʱδ��Ӧ
		//{
		//	if (GetTickCount() - single_tcp->tcp_msg_send[single_tcp->wait_for_ack_msg].time > RTT)
		//	{
		//		single_tcp->Threshold = single_tcp->cwnd / 2;
		//		single_tcp->cwnd = MSS;
		//		single_tcp->tcp_msg_send[single_tcp->wait_for_ack_msg].time = GetTickCount();
		//		//sendtoip(single_tcp->tcp_msg_send[single_tcp->MSG_ACK].tcpmessage, single_tcp->IP, ��һ������������ĵĳ���);
		//	}
		//	single_tcp = single_tcp->next;
		//}

		// ����tcp��������������������Ϣ
		struct tcplist *single_tcp = head->next;	// fuck your mom
		while (single_tcp != NULL)
		{
			if (single_tcp->connect_status == LINK_CONNECT_BIDIR || single_tcp->connect_status == LINK_PEER_HALF_OPEN)
			{
				// ���µݽ����Ķ�
				while (min(single_tcp->wait_for_fill, single_tcp->wait_for_send + MSS) - single_tcp->wait_for_ack <= min(single_tcp->cong_wind, single_tcp->rcvd_wind)
					&& single_tcp->wait_for_fill > single_tcp->wait_for_send)
				{
					int new_send;
					//new_send = min(single_tcp->wait_for_ack + min(single_tcp->cong_wind, single_tcp->rcvd_wind), min(single_tcp->wait_for_fill, single_tcp->wait_for_send + MSS));
					new_send = min(single_tcp->wait_for_fill, single_tcp->wait_for_send + MSS);
					single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].datalen = new_send - single_tcp->wait_for_send;
					single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].time = GetTickCount();
					single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].seq_number = single_tcp->wait_for_send;

					struct tcp_message new_tcp_msg;
					int datalen = new_send - single_tcp->wait_for_send;
					new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
					new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
					new_tcp_msg.tcp_seq_number = single_tcp->wait_for_send;	//seq_number can be deleted?
					new_tcp_msg.tcp_ack_number = single_tcp->send_ack_needed ? single_tcp->next_send_ack : 0;
					new_tcp_msg.tcp_hdr_length = 5;
					new_tcp_msg.tcp_reserved = 0;
					new_tcp_msg.tcp_urg = 0;
					new_tcp_msg.tcp_ack = single_tcp->send_ack_needed ? 1 : 0;
					new_tcp_msg.tcp_psh = 0;
					new_tcp_msg.tcp_rst = 0;
					new_tcp_msg.tcp_syn = 0;
					new_tcp_msg.tcp_fin = 0;
					new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
					new_tcp_msg.tcp_urg_ptr = NULL;
					memcpy(new_tcp_msg.tcp_opts_and_app_data, &single_tcp->tcp_buf_send[single_tcp->wait_for_send], single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].datalen);
					new_tcp_msg.tcp_checksum = tcpmakesum(datalen, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, datalen % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
							
					// ���������
					TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].datalen);

					single_tcp->wait_for_send = new_send;
					single_tcp->wait_for_fill_msg++;
					single_tcp->send_ack_needed = false;
				}

				if (single_tcp->send_ack_needed)
				{
					Temp_Send_ACK(single_tcp);
				}

				// ���Ͻ�������
				if (single_tcp->last_rcvd > single_tcp->last_read)
				{
					// ��������Ӧ�ò�Ľṹ��
					struct sockstruct *new_sockstruct = new sockstruct();
					new_sockstruct->dstport = single_tcp->tcp_src_port;
					new_sockstruct->srcport = single_tcp->tcp_dst_port;
					new_sockstruct->funcID = SOCKSEND;
					new_sockstruct->datalength = single_tcp->last_rcvd - single_tcp->last_read;
					IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
					IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
					// ---------��socket�ͷţ����д���ƺ���̫�淶--------------------------------
					new_sockstruct->data = (char *)malloc(new_sockstruct->datalength*sizeof(char));
					// ---------��socket�ͷţ����д���ƺ���̫�淶--------------------------------
					memcpy(new_sockstruct->data, single_tcp->tcp_buf_rcvd+single_tcp->last_read+1, new_sockstruct->datalength);

					// ����Ӧ�ò�
					AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);

					single_tcp->last_read = single_tcp->last_rcvd;
					// ����Ѿ��Ͻ�������ƺ�����Ҫ�������ӿ��԰�handin�ֶ�ɾ����
					// ��������tcpmsg_rcvd��û����ðɣ�����
				}
			}

			// ����SYN����һ������
			else if (single_tcp->connect_status == LINK_CONNECTING)
			{
				// ����SYN���Ķ�
				struct tcp_message new_tcp_msg;
				new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
				new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
				new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
				new_tcp_msg.tcp_ack_number = 0;
				new_tcp_msg.tcp_hdr_length = 5;
				new_tcp_msg.tcp_reserved = 0;
				new_tcp_msg.tcp_urg = 0;
				new_tcp_msg.tcp_ack = 0;
				new_tcp_msg.tcp_psh = 0;
				new_tcp_msg.tcp_rst = 0;
				new_tcp_msg.tcp_syn = 1;
				new_tcp_msg.tcp_fin = 0;
				new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
				new_tcp_msg.tcp_urg_ptr = NULL;
				new_tcp_msg.tcp_opts_and_app_data[0] = 33;	// whatever
				new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
				// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// �ı�tcp���ӵ�״̬���ȴ��Է�������syn����
				single_tcp->connect_status = LINK_WAIT_FOR_SYN;

				// ���»�����ָ�룬װ����TCP_send�������ݵ�״̬
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
			}

			// ����SYN��ACK���ڶ�������
			else if (single_tcp->connect_status == LINK_CONNECTED)
			{
				// ����ACK&SYN���Ķ�
				struct tcp_message new_tcp_msg;
				new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
				new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
				new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
				new_tcp_msg.tcp_ack_number = single_tcp->next_send_ack;
				new_tcp_msg.tcp_hdr_length = 5;
				new_tcp_msg.tcp_reserved = 0;
				new_tcp_msg.tcp_urg = 0;
				new_tcp_msg.tcp_ack = 1;
				new_tcp_msg.tcp_psh = 0;
				new_tcp_msg.tcp_rst = 0;
				new_tcp_msg.tcp_syn = 1;
				new_tcp_msg.tcp_fin = 0;
				new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
				new_tcp_msg.tcp_urg_ptr = NULL;
				new_tcp_msg.tcp_opts_and_app_data[0] = 33;	// whatever
				new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
				// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// �ı�tcp���ӵ�״̬���ȴ��Է�������synack����
				single_tcp->connect_status = LINK_WAIT_FOR_SYNACK;

				// ���»�����ָ�룬װ����TCP_send�������ݵ�״̬
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				// ֪ͨӦ�ò�Accept�¼�
				// ��������Ӧ�ò�Ľṹ��
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
				new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
				new_sockstruct->funcID = SOCKACCEPT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// ����Ӧ�ò�
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// ����SYNACK������������
			else if (single_tcp->connect_status == LINK_GOT_SYN)
			{
				// ����SYNACK���Ķ�
				struct tcp_message new_tcp_msg;
				new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
				new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
				new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
				new_tcp_msg.tcp_ack_number = single_tcp->next_send_ack;
				new_tcp_msg.tcp_hdr_length = 5;
				new_tcp_msg.tcp_reserved = 0;
				new_tcp_msg.tcp_urg = 0;
				new_tcp_msg.tcp_ack = 1;
				new_tcp_msg.tcp_psh = 0;
				new_tcp_msg.tcp_rst = 0;
				new_tcp_msg.tcp_syn = 0;
				new_tcp_msg.tcp_fin = 0;
				new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
				new_tcp_msg.tcp_urg_ptr = NULL;
				new_tcp_msg.tcp_opts_and_app_data[0] = 33;	// whatever
				new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
				// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// �ı�tcp���ӵ�״̬��������
				single_tcp->connect_status = LINK_CONNECT_BIDIR;

				// ���»�����ָ�룬װ����TCP_send�������ݵ�״̬
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				// ֪ͨӦ�ò���Է�����Դ��
				// ��������Ӧ�ò�Ľṹ��
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
				new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
				new_sockstruct->funcID = SOCKCONNECT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// ����Ӧ�ò�
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// ֪ͨӦ�ò������Դ
			else if (single_tcp->connect_status == LINK_GOT_SYNACK)
			{
				// �ı�tcp���ӵ�״̬��������
				single_tcp->connect_status = LINK_CONNECT_BIDIR;

				// ��������Ӧ�ò�Ľṹ��
				struct sockstruct *new_sockstruct=new sockstruct();
				new_sockstruct->dstport = single_tcp->tcp_src_port;
				new_sockstruct->srcport = single_tcp->tcp_dst_port;
				new_sockstruct->funcID = SOCKCONNECT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// ����Ӧ�ò�
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// �رձ������ӣ�����FIN
			else if (single_tcp->connect_status == LINK_FINISHING
					|| single_tcp->connect_status == LINK_CONNECT_DESTROYING)
			{
				// �ڷ������ݲ��õ��������ݵ�ȷ��֮ǰ�����Է���FIN
				if (single_tcp->wait_for_ack == single_tcp->wait_for_fill)
				{
					// ����FIN���Ķ�
					struct tcp_message new_tcp_msg;
					new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
					new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
					new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
					new_tcp_msg.tcp_ack_number = 0;
					new_tcp_msg.tcp_hdr_length = 5;
					new_tcp_msg.tcp_reserved = 0;
					new_tcp_msg.tcp_urg = 0;
					new_tcp_msg.tcp_ack = 0;
					new_tcp_msg.tcp_psh = 0;
					new_tcp_msg.tcp_rst = 0;
					new_tcp_msg.tcp_syn = 0;
					new_tcp_msg.tcp_fin = 1;
					new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
					new_tcp_msg.tcp_urg_ptr = NULL;
					new_tcp_msg.tcp_opts_and_app_data[0] = 21;	// whatever
					new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
					// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼fin
					TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

					// ���»�����ָ�룬װ����TCP_send�������ݵ�״̬
					++(single_tcp->seq_number);
					++(single_tcp->wait_for_fill);
					++(single_tcp->wait_for_ack);
					++(single_tcp->wait_for_send);

					if (single_tcp->connect_status == LINK_FINISHING)
					{
						// �ı�tcp���ӵ�״̬���ȴ��Է�������finack����
						single_tcp->connect_status = LINK_WAIT_FOR_FINACK;
					}
					else //if (single_tcp->connect_status == LINK_CONNECT_DESTROYING)
					{
						// �ı�tcp���ӵ�״̬���ȴ��Է�������finack����
						single_tcp->connect_status = LINK_WAIT_FOR_DESACK; 
					}
				}
			}

			// ����FIN��ACK
			else if (single_tcp->connect_status == LINK_FINISHED
					|| single_tcp->connect_status == LINK_CONNECT_DESTROYED)
			{
				// ����ACK���Ķ�
				struct tcp_message new_tcp_msg;
				new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
				new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
				new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
				new_tcp_msg.tcp_ack_number = single_tcp->next_send_ack;
				new_tcp_msg.tcp_hdr_length = 5;
				new_tcp_msg.tcp_reserved = 0;
				new_tcp_msg.tcp_urg = 0;
				new_tcp_msg.tcp_ack = 1;
				new_tcp_msg.tcp_psh = 0;
				new_tcp_msg.tcp_rst = 0;
				new_tcp_msg.tcp_syn = 0;
				new_tcp_msg.tcp_fin = 0;
				new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
				new_tcp_msg.tcp_urg_ptr = NULL;
				new_tcp_msg.tcp_opts_and_app_data[0] = 21;	// whatever
				new_tcp_msg.tcp_checksum = tcpmakesum(1, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 1, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
				// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// ���»�����ָ�룬װ����TCP_send�������ݵ�״̬
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				if (single_tcp->connect_status == LINK_FINISHED)
				{
					// �ı�tcp���ӵ�״̬����Ϊ�Է��뿪״̬
					single_tcp->connect_status = LINK_PEER_HALF_OPEN;
				}
				else if (single_tcp->connect_status == LINK_CONNECT_DESTROYED)
				{
					// �ı�tcp���ӵ�״̬����Ϊ˫��Ͽ�
					single_tcp->connect_status = LINK_CONNECT_LOSE;
				}
			}
			else
			{
				// nothing
			}

			// �Ͽ����ӣ�һ��Ҫ��if�������ܽ�����elseif
			if (single_tcp->connect_status == LINK_CONNECT_LOSE)
			{
				// ��������Ӧ�ò�Ľṹ��
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = single_tcp->tcp_src_port;
				new_sockstruct->srcport = single_tcp->tcp_dst_port;
				new_sockstruct->funcID = SOCKCLOSE;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// ����Ӧ�ò�
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);

				// ����TCP����
				deleteNode(single_tcp);
			}

			single_tcp = single_tcp->next;
		}
		//free(single_tcp);ɱ��֮��
	}
}

//int Count_ACK(int ACK_global)         //�յ�3������ACK����global_TCP_resend_flag��ֵ
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

//int Fastretransmit(int ACK_global)    //������Ҫ�ط���ACK���
//{
//	int retransmitACKID;
//	retransmitACKID = ACK_global;
//	return retransmitACKID;
//}

//int Wrongretrasnsmit(int ACK_global, u16 len_tcp, u16 src_port, u16 dest_port, bool padding, u16 *buff, u16 checksum) //������Ҫ�ط���ACK���,-1��ʾ����Ҫ�ش�
//{
//	int sum;
//	sum = tcpmakesum(len_tcp, src_port, dest_port, padding, buff) + checksum;
//	if (sum != 0xffff)                                                                                    //��������
//	{
//		return ACK_global;
//	}
//	else
//	{
//		return -1;
//	}
//}

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
}

void TCP_Send2IP(struct tcp_message send_tcp_message, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len)
{
	struct Msg *new_ip_msg = new Msg();
	new_ip_msg->sip = src_ip;
	new_ip_msg->dip = dst_ip;
	new_ip_msg->ih_sport = send_tcp_message.tcp_src_port;
	new_ip_msg->ih_dport = send_tcp_message.tcp_dst_port;
	new_ip_msg->datelen = data_len+4*send_tcp_message.tcp_hdr_length;
	memcpy(new_ip_msg->data, &send_tcp_message, new_ip_msg->datelen);
	new_ip_msg->protocol = PROTOCOL_TCP;	// 6 for TCP

	// ���������
	AfxGetApp()->m_pMainWnd->PostMessage(IPTOLINK, (WPARAM)new_ip_msg);
}

void UDP_Send2IP(struct sockstruct data_from_applayer, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len)
{
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;

	struct udp_message new_udp_msg;
	// ����UDP���Ķνṹ
	new_udp_msg.udp_src_port = src_port;
	new_udp_msg.udp_dst_port = dst_port;
	new_udp_msg.udp_msg_length = 8 + data_len;
	memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len + 1); // +1 for \0
	new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len % 2, (u16 *)&(new_udp_msg.udp_app_data));

	// UDP��ӵ������
	struct Msg *new_ip_msg = new Msg();
	new_ip_msg->sip = src_ip;
	new_ip_msg->dip = dst_ip;
	new_ip_msg->ih_sport = src_port;
	new_ip_msg->ih_dport = dst_port;
	new_ip_msg->datelen = new_udp_msg.udp_msg_length;
	memcpy(new_ip_msg->data, &new_udp_msg, new_ip_msg->datelen);
	new_ip_msg->protocol = PROTOCOL_UDP;	// 17 for UDP

	// ���������
	AfxGetApp()->m_pMainWnd->PostMessage(IPTOLINK, (WPARAM)new_ip_msg);
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

int wait_for_handshaking_ack(struct tcplist *tcp)
{
	for (;;)
	{
		if (tcp->tcp_established_syn_seq != -1)
		{
			break;
		}
	}
	return tcp->tcp_established_syn_seq+1;
}

void port_listen(unsigned short port)
{
	listening_flag[port] = true;
}

bool check_listening(unsigned short port)
{
	return listening_flag[port];
}

void fill_new_tcplist(struct tcplist *new_tcp)
{
	new_tcp->next = NULL;
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
	new_tcp->cong_status = CONG_SS;
	new_tcp->tcp_established_syn_seq = -1;
}


void sandw_send(struct sockstruct data_from_applayer)   
{
	while (global_sandw_send_flag);
	global_send_sockstruct_sandw = data_from_applayer;
	global_sandw_send_flag = true;
	while (global_sandw_send_flag);
}

void sandw_receive(struct Msg data_from_netlayer)
{
	while (global_sandw_receive_flag);
	global_receive_ip_msg_sandw = data_from_netlayer;
	global_sandw_receive_flag = true;
	while (global_sandw_receive_flag);
}

void stop_wait()   //ֹͣ�ȴ�
{
	srand(time(0));
	global_sandw_send_flag = global_sandw_receive_flag = global_sandw_sendtoIP_flag = false;
	global_sandw_sendtoIP_flag = true;
	new_sandw.time = 0;
	new_sandw.last_waitforsend_msg = 0;
	new_sandw.last_send_msg = 0;
	new_sandw.last_ack = 1;
	for (;;)
	{

		if (global_sandw_send_flag)   //����յ�Ӧ�ò㴫�����ı���
		{
			new_sandw.send_buf[new_sandw.last_waitforsend_msg] = global_send_sockstruct_sandw;  //���ñ��ķ��뷢�ͻ�����
			new_sandw.last_waitforsend_msg++;
			global_sandw_send_flag = false;
		}

		if (global_sandw_receive_flag)   //����յ��²㴫�����ı���
		{
			struct tcp_message new_tcp_msg;
			memcpy(&new_tcp_msg, global_receive_ip_msg_sandw.data, global_receive_ip_msg_sandw.datelen);

			// opts��dataһͬ���м���
			unsigned opts_data_len = global_receive_ip_msg_sandw.datelen - 20;

			// �����
			if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			{
				// ��������
				global_sandw_receive_flag = false;
				break;
			}

			if (new_tcp_msg.tcp_ack)   //�����Ӧ����ack
			{
				if (new_tcp_msg.tcp_ack_number == new_sandw.last_send.tcp_seq_number)  //����յ���ACKȷ�Ϻźͷ��͵ı��������ͬ
				{
					new_sandw.last_ack = new_tcp_msg.tcp_ack_number;
					global_sandw_sendtoIP_flag = true;

				}
				//�������֮ǰ���͵��Ǹ����ĵ�Ӧ���ģ�����������������
			}

			if (new_tcp_msg.tcp_ack == 0 || global_receive_ip_msg_sandw.datelen > 20)  //����յ��Ĳ���һ��Ӧ���ģ����Ǳ��˷�������һ�����ĵĻ���Ҫ��һ��ack��ȥ
			{
				struct tcp_message new_tcp_message;
				new_tcp_message.tcp_src_port = new_tcp_msg.tcp_dst_port;   //ack��Դ�˿ںź�Ŀ�Ķ˿ں����յ��ı����෴
				new_tcp_message.tcp_dst_port = new_tcp_msg.tcp_src_port;
				new_tcp_message.tcp_seq_number = 0;
				new_tcp_message.tcp_ack_number = new_tcp_msg.tcp_seq_number;
				new_tcp_message.tcp_hdr_length = 5;
				new_tcp_msg.tcp_reserved = 0;
				new_tcp_msg.tcp_urg = 0;
				new_tcp_msg.tcp_ack = 1;
				new_tcp_msg.tcp_psh = 0;
				new_tcp_msg.tcp_rst = 0;
				new_tcp_msg.tcp_syn = 0;
				new_tcp_msg.tcp_fin = 0;
				new_tcp_msg.tcp_rcv_window = 0;
				new_tcp_msg.tcp_urg_ptr = NULL;
				new_tcp_msg.tcp_checksum = 0;

				TCP_Send2IP(new_tcp_message, global_receive_ip_msg_sandw.dip, global_receive_ip_msg_sandw.sip, 20);

			}

			/*�����յ��ı��ķ�װ�����ϲ�APP*/
			struct sockstruct *new_sockstruct = new sockstruct();
			new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
			new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
			new_sockstruct->funcID = SOCKSEND;
			new_sockstruct->datalength = global_receive_ip_msg_sandw.datelen - 20;
			IP_uint2chars(new_sockstruct->srcip, new_tcp_msg.tcp_src_port);
			IP_uint2chars(new_sockstruct->dstip, new_tcp_msg.tcp_dst_port);
			// ---------��socket�ͷţ����д���ƺ���̫�淶--------------------------------
			new_sockstruct->data = (char *)malloc(new_sockstruct->datalength*sizeof(char));
			// ---------��socket�ͷţ����д���ƺ���̫�淶--------------------------------
			memcpy(new_sockstruct->data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct->datalength);

			// ����Ӧ�ò�	
			AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);



			global_sandw_receive_flag = false;
		}


		if (global_sandw_sendtoIP_flag)    //�յ�Ҫ�ӷ��ͻ�����ȡһ�����ķ��ͳ�ȥ���ź�
		{
			if (new_sandw.last_waitforsend_msg > new_sandw.last_send_msg)  //�������������û�з��͵ı���
			{
				struct tcp_message new_tcp_message;
				new_tcp_message.tcp_src_port = new_sandw.send_buf[new_sandw.last_send_msg].srcport;
				new_tcp_message.tcp_dst_port = new_sandw.send_buf[new_sandw.last_send_msg].dstport;
				new_tcp_message.tcp_seq_number = 1 - new_sandw.last_ack;
				new_tcp_message.tcp_ack_number = 0;
				new_tcp_message.tcp_hdr_length = 5;
				new_tcp_message.tcp_reserved = 0;
				new_tcp_message.tcp_urg = 0;
				new_tcp_message.tcp_ack = 0;
				new_tcp_message.tcp_psh = 0;
				new_tcp_message.tcp_rst = 0;
				new_tcp_message.tcp_syn = 0;
				new_tcp_message.tcp_fin = 0;
				new_tcp_message.tcp_rcv_window = 0;
				new_tcp_message.tcp_urg_ptr = NULL;
				memcpy(new_tcp_message.tcp_opts_and_app_data, new_sandw.send_buf[new_sandw.last_send_msg].data, new_sandw.send_buf[new_sandw.last_send_msg].datalength);

				int datalen = 20 + new_sandw.send_buf[new_sandw.last_send_msg].datalength;
				new_tcp_message.tcp_checksum = tcpmakesum(datalen, new_tcp_message.tcp_src_port, new_tcp_message.tcp_dst_port, datalen % 2, (u16 *)&(new_tcp_message.tcp_opts_and_app_data));

				//���ñ��ķ��͵�IP��
				TCP_Send2IP(new_tcp_message, IP_chars2uint(new_sandw.send_buf[new_sandw.last_send_msg].srcip), IP_chars2uint(new_sandw.send_buf[new_sandw.last_send_msg].dstip), datalen);

				new_sandw.last_send = new_tcp_message;    //��¼��η��ͳ�ȥ�ı��ģ�Ϊ֮��ʱ�ش�������
				new_sandw.time = GetTickCount();           //���¼�¼����ʱ��
				new_sandw.last_send_msg++;                //���ͻ��������Ѿ����͵ı����±��һ
			}
			global_sandw_sendtoIP_flag = false;
		}

		//��ʱ�ش�new_sandw.last_send

	}
}



//����Ϊѡ���ش������Ľ������ϲ�ʹ��ݸ��²�Ĳ��ֶ�ûд

struct SR_message new_SR;


void SR_send(struct sockstruct data_from_applayer)
{
	while (global_SR_send_flag);
	global_send_sockstruct_SR = data_from_applayer;
	global_SR_send_flag = true;
	while (global_SR_send_flag);
}

void SR_receive(struct Msg data_from_netlayer)
{
	while (global_SR_receive_flag);
	global_receive_ip_msg_SR = data_from_netlayer;
	global_SR_receive_flag = true;
	while (global_SR_receive_flag);
}

void _SR() //ѡ���ش�
{
	srand(time(0));//��ʱδ����
	global_SR_send_flag = global_SR_receive_flag = false;
	global_SR_send2IP_flag = true;
	new_SR.time = 0;
	new_SR.rcv_wnd = 0;
	new_SR.send_wnd = 0;
	new_SR.last_send_msg = 0;
	new_SR.last_rcv_msg = 0;
	for (int i = 0; i < SEND_STRUCT_SIZE; i++)
	{
		new_SR.state_send[i] = 0;
		new_SR.state_rcv[i] = 0;
	}

	for (;;)
	{
		if (global_SR_send_flag)
		{
			new_SR.send_buf[new_SR.last_waitforsend_msg] = global_send_sockstruct_SR;
			new_SR.last_waitforsend_msg++;
			global_SR_send_flag = false;
		}

		if (global_SR_receive_flag)
		{
			struct tcp_message new_tcp_msg;
			memcpy(&new_tcp_msg, global_receive_ip_msg_SR.data, global_receive_ip_msg_SR.datelen);

			unsigned opts_data_len = global_receive_ip_msg_SR.datelen - 20;

			// �����
			if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			{
				// ��������
				global_SR_receive_flag = false;
				break;
			}

			if (new_tcp_msg.tcp_ack)
			{
				if (new_tcp_msg.tcp_ack_number >= new_SR.send_wnd && new_tcp_msg.tcp_ack_number < new_SR.send_wnd + wnd_SR)
				{
					new_SR.state_send[new_tcp_msg.tcp_ack_number] = 1;
				}
			}
			else
			{
				if (new_tcp_msg.tcp_seq_number >= new_SR.rcv_wnd && new_tcp_msg.tcp_seq_number < new_SR.rcv_wnd + wnd_SR)
				{
					if (new_SR.state_rcv[new_tcp_msg.tcp_seq_number] == 0)
					{
						new_SR.rcv_buf[new_SR.last_rcv_msg] = global_receive_ip_msg_SR;
						new_SR.last_rcv_msg++;
						new_SR.state_rcv[new_tcp_msg.tcp_seq_number] = 1;
						struct tcp_message new_tcp_message;
						new_tcp_message.tcp_src_port = new_SR.send_buf[new_SR.last_send_msg - 1].dstport;
						new_tcp_message.tcp_dst_port = new_SR.send_buf[new_SR.last_send_msg - 1].srcport;
						new_tcp_message.tcp_ack_number = new_SR.last_send_msg - 1;
						new_tcp_message.tcp_ack = 1;
					}
				}
				else if (new_tcp_msg.tcp_seq_number >= new_SR.rcv_wnd - wnd_SR && new_tcp_msg.tcp_seq_number < new_SR.rcv_wnd)
				{
					struct tcp_message new_tcp_message;
					new_tcp_message.tcp_src_port = new_SR.send_buf[new_SR.last_send_msg - 1].dstport;
					new_tcp_message.tcp_dst_port = new_SR.send_buf[new_SR.last_send_msg - 1].srcport;
					new_tcp_message.tcp_ack_number = new_SR.last_send_msg - 1;
					new_tcp_message.tcp_ack = 1;
				}
			}
		}


		if (new_SR.state_send[new_SR.send_wnd + 1] == 1)
		{
			new_SR.send_wnd++;
		}

		if (new_SR.last_send_msg < new_SR.send_wnd + wnd_SR)
		{
			global_SR_send2IP_flag = true;
			new_SR.last_send_msg++;
		}

		if (new_SR.state_rcv[new_SR.rcv_wnd + 1] == 1)
		{
			new_SR.rcv_wnd++;
			//����new_SR.rcv_buf[rcv_wnd]�еı��ġ���
		}

		if (global_SR_send2IP_flag)
		{
			struct tcp_message new_tcp_message;
			new_tcp_message.tcp_src_port = new_SR.send_buf[new_SR.last_send_msg - 1].srcport;
			new_tcp_message.tcp_dst_port = new_SR.send_buf[new_SR.last_send_msg - 1].dstport;
			new_tcp_message.tcp_hdr_length = 5;
			new_tcp_message.tcp_seq_number = new_SR.last_send_msg - 1;
			memcpy(new_tcp_message.tcp_opts_and_app_data, new_SR.send_buf[new_SR.last_send_msg - 1].data, new_SR.send_buf[new_SR.last_send_msg - 1].datalength);
			int datalen = 20 + new_SR.send_buf[new_SR.last_send_msg - 1].datalength;
			new_tcp_message.tcp_checksum = tcpmakesum(datalen, new_tcp_message.tcp_src_port, new_tcp_message.tcp_dst_port, datalen % 2, (u16*)&(new_tcp_message.tcp_opts_and_app_data));

			new_SR.time = GetTickCount();
			global_SR_send2IP_flag = false;
		}

	}
}

void Temp_Send_ACK(struct tcplist *single_tcp)
{
	// ����ACK���Ķ�
	struct tcp_message new_tcp_msg;
	new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
	new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
	new_tcp_msg.tcp_seq_number = single_tcp->seq_number;
	new_tcp_msg.tcp_ack_number = single_tcp->next_send_ack;
	new_tcp_msg.tcp_hdr_length = 5;
	new_tcp_msg.tcp_reserved = 0;
	new_tcp_msg.tcp_urg = 0;
	new_tcp_msg.tcp_ack = 1;
	new_tcp_msg.tcp_psh = 0;
	new_tcp_msg.tcp_rst = 0;
	new_tcp_msg.tcp_syn = 0;
	new_tcp_msg.tcp_fin = 0;
	new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
	new_tcp_msg.tcp_urg_ptr = NULL;
	//new_tcp_msg.tcp_opts_and_app_data = NULL;	// whatever
	new_tcp_msg.tcp_checksum = tcpmakesum(0, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, 0, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));
			
	// ����TCP_send()��������TCP���Ľṹ�ͱ��Ļ��壬��Ϊ����δ��¼ACK
	TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 0);

	single_tcp->send_ack_needed = false;
}

void time_over()
{
	for (;;)
	{
		struct tcplist *single_tcp = head->next;
		while (single_tcp != NULL)
		{
			int temp_msg_num;
			//�������tcp���ѷ��͵�δȷ�ϵ���Щ�����Ƿ�ʱ
			for (temp_msg_num = single_tcp->wait_for_ack_msg; temp_msg_num < single_tcp->wait_for_fill_msg; temp_msg_num++)
			{
				if (GetTickCount() - single_tcp->tcp_msg_send[temp_msg_num].time > RTT)
				{
					//��ʱӵ������
					single_tcp->threshold = single_tcp->cong_wind / 2;
					single_tcp->cong_wind = MSS;
					single_tcp->tcp_msg_send[temp_msg_num].time = GetTickCount();
					//����Ϊ��ʱ�ش��ѷ��͵�δȷ�ϵĵ�һ������
					struct tcp_message new_tcp_msg;
					new_tcp_msg.tcp_src_port = single_tcp->tcp_src_port;
					new_tcp_msg.tcp_dst_port = single_tcp->tcp_dst_port;
					new_tcp_msg.tcp_seq_number = single_tcp->wait_for_ack; // q_number can be deleted ?
						new_tcp_msg.tcp_ack_number = single_tcp->send_ack_needed ? single_tcp->next_send_ack : 0;
					new_tcp_msg.tcp_hdr_length = 5;
					new_tcp_msg.tcp_reserved = 0;
					new_tcp_msg.tcp_urg = 0;
					new_tcp_msg.tcp_ack = single_tcp->send_ack_needed ? 1 : 0;
					new_tcp_msg.tcp_psh = 0;
					new_tcp_msg.tcp_rst = 0;
					new_tcp_msg.tcp_syn = 0;
					new_tcp_msg.tcp_fin = 0;
					new_tcp_msg.tcp_rcv_window = single_tcp->rcvd_wind;
					new_tcp_msg.tcp_urg_ptr = NULL;
					memcpy(new_tcp_msg.tcp_opts_and_app_data, &single_tcp->tcp_buf_send[single_tcp->wait_for_ack], single_tcp->tcp_msg_send[temp_msg_num].datalen);
					new_tcp_msg.tcp_checksum = tcpmakesum(single_tcp->tcp_msg_send[temp_msg_num].datalen, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, single_tcp->tcp_msg_send[temp_msg_num].datalen % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data));

					// ���������
					TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, single_tcp->tcp_msg_send[temp_msg_num].datalen);
				}
			}

			if (single_tcp->send_ack_needed)
			{
				if (GetTickCount() - single_tcp->receive_time > 500)
				{
					Temp_Send_ACK(single_tcp);
				}
			}
			single_tcp = single_tcp->next;
		}
	}
}
