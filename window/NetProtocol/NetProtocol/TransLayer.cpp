#pragma once
#include "stdafx.h"
#include "TransLayer.h"

struct tcplist tcp_list;
tcplist* head = NULL;

int ACK_global;
int RTT;

static float EstimatedRTT;
static float DevRTT;
static int CountACK = 0;//����ACK��������
static int ACK_Now = -1;

bool createNodeList()
{
	head = (tcplist*)malloc(sizeof(tcp_list));
	if (NULL == head)
	{
		return false;
	}
	else
	{
		head->MSG_num = 0;
		head->cwnd = MSS;
		head->IP = 0;
		head->PORT = 0;
		head->Threshold = 65 * 1024;
		head->MSG_ACK = 0;
		head->next = NULL;
		return true;
	}
}

bool addNode(tcplist* tcp_list)
{
	if (NULL == head)
	{
		return false;
	}
	tcplist* p = head->next;
	tcplist* q = head;
	while (NULL != p)
	{
		q = p;
		p = p->next;
	}
	q->next = tcp_list;
	tcp_list->next = NULL;
	return true;
}

bool deletenode(tcplist* p)
{
	tcplist* s1;
	tcplist* s2;
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

struct tcplist *getNode(unsigned int ip, unsigned short port)
{//�ڴ�ͷ���ĵ�����head�в��ҵ�i����㣬���ҵ���0��i��n����
	//�򷵻ظý��Ĵ洢λ�ã����򷵻�NULL��
	tcplist *p;
	p = head;//��ͷ��㿪ʼɨ��
	while (p)
	{//˳ָ�����ɨ�裬ֱ��p->nextΪNULLΪֹ
		if (p->IP == ip && p->PORT == port)  //���ҵ�Ŀ��IP���򷵻�p
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

bool global_TCP_new_flag;

bool global_TCP_send_flag;

bool global_TCP_receive_flag;
bool global_TCP_resend_flag;
bool global_TCP_destroy_flag;

struct tcp_message global_new_tcp_msg;
unsigned int global_ip;
unsigned short global_port;
int global_datalen;

void TCP_new(unsigned int ip_temp, unsigned short port_temp)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_new_flag);
	global_TCP_new_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
}

void TCP_send(unsigned int ip_temp, unsigned short port_temp, struct tcp_message tcp_msg_temp, int datalen_temp)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_send_flag);
	global_TCP_send_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
	global_new_tcp_msg = tcp_msg_temp;
	global_datalen = datalen_temp;
}

void TCP_receive(unsigned int ip_temp, unsigned short port_temp, struct tcp_message tcp_msg_temp, int datalen_temp)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_receive_flag);
	global_TCP_receive_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
	global_new_tcp_msg = tcp_msg_temp;
	global_datalen = datalen_temp;
}

void TCP_resend()
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_resend_flag);
	global_TCP_resend_flag = true;
}

void TCP_destroy(unsigned int ip_temp, unsigned short port_temp)
{
	// ��ѯ����Ϊ���ٿ��ڴ����ٶȣ���ѯЧ�ʷ�����
	while (global_TCP_destroy_flag);
	global_TCP_destroy_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
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
			tcplist *node1 = (tcplist *)malloc(sizeof(tcp_list));
			node1->next = NULL;
			node1->cwnd = MSS;
			node1->IP = global_ip;
			node1->PORT = global_port;
			node1->MSG_ACK = 0;
			node1->MSG_sum = 0;
			node1->MSG_num = 0;
			node1->send_size = 0;
			node1->Threshold = INITIAL_THRESHOLD;
			node1->LastByteRcvd = 0;
			node1->LastByteRead = 0;  
			node1->rec_size = 0;
			node1->RcvWindow = Rcvbuffer;
			node1->seq_number = rand()%RANDOM_SEQ_NUMBER_MODULE;
			node1->ACK_count = 0;
			node1->last_ACK = 0;
			addNode(node1);
			global_TCP_new_flag = false;
		}

		// �Ƿ���Ҫ����һ��TCP���ģ����ﲢ���������ķ��ͣ����Ǽ�������ͱ��Ķ���
		// �ȴ�����ʱ�����µݽ�
		if (global_TCP_send_flag)
		{
			// �½���ӦTCP���ӵ�Msg
			tcplist *temp1 = getNode(global_ip, global_port);
			temp1->tcp_msg_send[temp1->MSG_sum].tcpmessage = global_new_tcp_msg;
			temp1->tcp_msg_send[temp1->MSG_sum].datalen = global_datalen;
			++(temp1->MSG_sum);
			if (temp1->MSG_sum >= SEND_BUFFER_SIZE)
			{
				temp1->MSG_sum = 0;
			}
			// �����ʹ�С����
			temp1->send_size += global_datalen;
			global_TCP_send_flag = false;
		}

		// �Ƿ���Ҫ����һ��TCP���ģ�������㽻���ı���������ջ��棬�ȴ�����
		// ʱ�����Ͻ���
		if (global_TCP_receive_flag)
		{
			// ���¶�ӦTCP��Msg��window��ack
			tcplist *temp1 = getNode(global_ip, global_port);
			temp1->tcp_msg_rcvd[temp1->LastByteRcvd].tcpmessage = global_new_tcp_msg;
			temp1->tcp_msg_rcvd[temp1->LastByteRcvd].datalen = global_datalen;
			++(temp1->LastByteRcvd);
			if (temp1->LastByteRcvd >= RCVD_BUFFER_SIZE)
			{
				temp1->LastByteRcvd = 0;
			}
			// ���մ��ڸ���
			temp1->RcvWindow -= global_datalen;

			// ����RTT
			RTT = (int)getSampleRTT(GetTickCount(), temp1->tcp_msg_send[temp1->MSG_ACK].time);

			//�Ƿ���ACK
			if (global_new_tcp_msg.tcp_ack != 0)
			{
				ACK_global = global_new_tcp_msg.tcp_ack_number;  //����ACK����ǰACKֵ
				tcplist *temp2; 
				temp2 = getNode(global_ip, global_port);

				// ����ack������
				if (temp1->last_ACK == global_new_tcp_msg.tcp_ack_number)
				{
					++(temp1->ACK_count);
				}
				// ������ack��ȷ��
				else
				{
					temp1->last_ACK = global_new_tcp_msg.tcp_ack_number;
				}

				//if (temp2->tcp_msg_send[temp2->MSG_ACK].tcpmessage.tcp_seq_number >= ACK_global)   //����ACK����
				//{
				//	temp2->tcp_msg_send[temp2->MSG_ACK].ACK++;  
				//}
				//else
				//{
				//	temp2->MSG_ACK++;      //��ǰ��ȷ�ϵı��ĵõ�ȷ���ˣ��±�������ָ����һ������ȷ�ϵ��Ѿ����͵ı���
				//}

				if (temp2->cwnd <= temp2->Threshold) //������
				{
					temp2->cwnd += MSS;
				}
				else
				{
					//if (temp2->tcp_msg_send[temp2->MSG_ACK].ACK >= 3)    //�յ�3������ACK������Ϊӵ������
					//{
					//	temp2->Threshold = temp2->cwnd / 2;
					//	temp2->cwnd = temp2->Threshold;
					//	//sendtoip(temp3->tcp_msg_send[temp2->MSG_ACK].tcpmessage, temp2->IP, ��һ������������ĵĳ���);
					//}
					//else      //�յ�ǰ��δȷ�����ݵ�ACK
					//{
					//	temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
					//}
				}
				ACK_global = 0;


			}

			global_TCP_send_flag = false;

		}

		// �ش�..�е����⣿
		if (global_TCP_resend_flag)
		{
		}

		// �Ƿ���Ҫ���һ��TCP����
		if (global_TCP_destroy_flag)
		{
			// ���TCP����
			deletenode(getNode(global_ip, global_port));
			global_TCP_destroy_flag = false;
		}

 



		tcplist* temp3 = head;
		while (temp3)         //ʵʱ���ÿ��TCP�µ�ǰ������Ӧ�ı����Ƿ�ʱδ��Ӧ
		{
			if (GetTickCount() - temp3->tcp_msg_send[temp3->MSG_ACK].time > RTT)
			{
				temp3->Threshold = temp3->cwnd / 2;
				temp3->cwnd = MSS;
				temp3->tcp_msg_send[temp3->MSG_ACK].time = GetTickCount();
				//sendtoip(temp3->tcp_msg_send[temp3->MSG_ACK].tcpmessage, temp3->IP, ��һ������������ĵĳ���);
			}
			temp3 = temp3->next;
		}
		//



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