#pragma once
#include "stdafx.h"
#include "TransLayer.h"

struct tcplist tcp_list;
tcplist* head = NULL;

struct tcp_message global_new_tcp_msg;

int ACK_global;

unsigned int global_ip;
unsigned short global_port;

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
		head->count = 0;
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
{//ÔÚ´øÍ·½áµãµÄµ¥Á´±íheadÖÐ²éÕÒµÚi¸ö½áµã£¬ÈôÕÒµ½£¨0¡Üi¡Ün£©£¬
	//Ôò·µ»Ø¸Ã½áµãµÄ´æ´¢Î»ÖÃ£¬·ñÔò·µ»ØNULL¡£
	tcplist *p;
	p = head;//´ÓÍ·½áµã¿ªÊ¼É¨Ãè
	while (p)
	{//Ë³Ö¸ÕëÏòºóÉ¨Ãè£¬Ö±µ½p->nextÎªNULLÎªÖ¹
		if (p->IP == ip && p->PORT == port)  //ÈôÕÒµ½Ä¿±êIP£¬Ôò·µ»Øp
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

void TCP_new(unsigned int ip_temp, unsigned short port_temp)
{
	global_TCP_new_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
}

void TCP_send(unsigned int ip_temp, unsigned short port_temp, struct tcp_message tcp_msg_temp)
{
	global_TCP_send_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
	global_new_tcp_msg = tcp_msg_temp;
}

void TCP_receive(unsigned int ip_temp, unsigned short port_temp, int ACK_temp)
{
	global_TCP_receive_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
	ACK_global = ACK_temp;
}

void TCP_resend()
{
	global_TCP_resend_flag = true;
}

void TCP_destroy(unsigned int ip_temp, unsigned short port_temp)
{
	global_TCP_destroy_flag = true;
	global_ip = ip_temp;
	global_port = port_temp;
}



void TCP_controller()
{
	// µ¥Ïß³Ì×Ü¿ØµÄÁ÷³Ì
	createNodeList();
	global_TCP_new_flag = global_TCP_send_flag = global_TCP_resend_flag = global_TCP_receive_flag = global_TCP_destroy_flag = false;
	for (;;)
	{
		if (global_TCP_new_flag)
		{
			tcplist* node1 = (tcplist*)malloc(sizeof(tcp_list));
			node1->MSG_num = 1;
			node1->cwnd = MSS;
			node1->IP = global_ip;
			node1->PORT = global_port;
			node1->count = 0;
			node1->Threshold = 65 * 1024;
			node1->tcp_msg[node1->MSG_num - 1].ACK = 0;
			mescopy(node1->tcp_msg[node1->MSG_num - 1].tcpmessage, global_new_tcp_msg);
			node1->tcp_msg[node1->MSG_num - 1].time = GetTickCount();
			node1->next = NULL;
			addNode(node1);
			global_TCP_new_flag = false;
		}

		if (global_TCP_send_flag)
		{
			// ÐÂ½¨¶ÔÓ¦TCPÁ¬½ÓµÄMsg

			tcplist *temp1;
			temp1 = getNode(global_ip, global_port);
			temp1->MSG_num++;
			temp1->tcp_msg[temp1->MSG_num - 1].ACK = 0;
			mescopy(temp1->tcp_msg[temp1->MSG_num - 1].tcpmessage, global_new_tcp_msg);
			temp1->tcp_msg[temp1->MSG_num - 1].time = GetTickCount();
			global_TCP_send_flag = false;
		}

		if (global_TCP_receive_flag)
		{
			// ¸üÐÂ¶ÔÓ¦TCPºÍMsgµÄwindowºÍack
			tcplist* temp2;
			temp2 = getNode(global_ip, global_port);
			temp2->tcp_msg[temp2->count].time = GetTickCount();
			if (temp2->tcp_msg[temp2->count].tcpmessage.tcp_seq_number >= ACK_global)   //ÈßÓàACK¼ÆÊý
			{
				temp2->tcp_msg[temp2->count].ACK++;
			}
			else
			{
				temp2->count++;
			}
			if (temp2->cwnd <= temp2->Threshold) //ÂýÆô¶¯
			{
				temp2->cwnd += MSS;
			}
			else
			{
				if (temp2->tcp_msg[temp2->count].ACK >= 3)    //ÊÕµ½3¸öÈßÓàACK£¬ÉèÖÃÎªÓµÈû±ÜÃâ
				{
					temp2->Threshold = temp2->cwnd / 2;
					temp2->cwnd = temp2->Threshold;
				}
				else      //ÊÕµ½Ç°ÃæÎ´È·ÈÏÊý¾ÝµÄACK
				{
					temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
				}
			}
			ACK_global = 0;
			global_TCP_receive_flag = false;
		}

		if (global_TCP_resend_flag)
		{
			// ¿ìËÙÖØ´«£¬Í¨ÖªTrans2IP

		}

		if (global_TCP_destroy_flag)
		{
			// ²ð³ýTCPÁ¬½Ó
			deletenode(getNode(global_ip, global_port));
			global_TCP_destroy_flag = false;
		}

		//if (true/*ÒªÇó·¢ËÍ±¨ÎÄ*/)
		//{
		//	tcplist *temp1;
		//	temp1 = getNode(global_ip,global_port);  //ÇëÇó±¨ÎÄµÄÔ´ip(+¶Ë¿ÚºÅ)
		//	if (temp1 == NULL)   //Èç¹ûÇëÇó±¨ÎÄµÄÔ´ip¶ÔÓ¦µÄTCPµ±Ç°Î´½¨Á¢Á¬½Ó£¬ÔòÐÂ½¨Ò»¸öTCP£¬¼ÓÈëÁ´±íÎ²²¿
		//	{
		//		tcplist* node1 = (tcplist*)malloc(sizeof(tcp_list));
		//		node1->MSG_num = 1;
		//		node1->cwnd = MSS;
		//		node1->IP = global_ip;
		//		node1->PORT = global_port;
		//		node1->count = 0;
		//		node1->Threshold = 65 * 1024;
		//		node1->tcp_msg[node1->MSG_num - 1].ACK = 0;
		//		node1->tcp_msg[node1->MSG_num - 1].tcpmessage = global_new_tcp_msg;
		//		node1->tcp_msg[node1->MSG_num - 1].time = GetTickCount();
		//		node1->next = NULL;
		//		addNode(node1);
		//	}
		//	else     //Èç¹ûÇëÇó±¨ÎÄµÄÔ´IP¶ÔÓ¦µÄTCP¶Ë¿ÚÒÑ½¨Á¢Á¬½Ó£¬Ôò¸ù¾Ý±¨ÎÄÄÚÈÝ£¬ÌîÐ´µ±Ç°TCP¶Ë¿ÚµÄtcp_msg½á¹¹£¨¼ÇÂ¼±¨ÎÄÏà¹Ø£©
		//	{
		//		if (temp1->MSG_num - temp1->count <= temp1->cwnd / MSS)
		//		{
		//			temp1->MSG_num++;
		//			temp1->tcp_msg[temp1->MSG_num-1].ACK = 0;
		//			temp1->tcp_msg[temp1->MSG_num - 1].tcpmessage = global_new_tcp_msg;
		//			temp1->tcp_msg[temp1->MSG_num-1].time = GetTickCount();
		//		}
		//	}
		//}

		tcplist* temp3 = head;
		while (temp3)         //ÊµÊ±¼ì²éÃ¿¸öTCPÏÂµ±Ç°Õý´ýÏìÓ¦µÄ±¨ÎÄÊÇ·ñ³¬Ê±Î´ÏìÓ¦
		{
			if (GetTickCount() - temp3->tcp_msg[temp3->count].time > RTT)
			{
				temp3->Threshold = temp3->cwnd / 2;
				temp3->cwnd = MSS;
				temp3->tcp_msg[temp3->count].time = GetTickCount();
			}
			temp3 = temp3->next;
		}
		//if (ACK_global != 0)
		//{
		//	//µÃµ½ÏìÓ¦±¨ÎÄµÄÄ¿±êip(+¶Ë¿ÚºÅ)
		//	tcplist* temp2;
		//	temp2 = getNode(global_ip, global_port);
		//	temp2->tcp_msg[temp2->count].time = GetTickCount();
		//	if (temp2->tcp_msg[temp2->count].tcpmessage.tcp_seq_number >= ACK_global)   //ÈßÓàACK¼ÆÊý
		//	{
		//		temp2->tcp_msg[temp2->count].ACK++;
		//	}
		//	else
		//	{
		//		temp2->count++;
		//	}
		//	if (temp2->cwnd <= temp2->Threshold) //ÂýÆô¶¯
		//	{
		//		temp2->cwnd += MSS;
		//	}
		//	else
		//	{
		//		if (temp2->tcp_msg[temp2->count].ACK >= 3)    //ÊÕµ½3¸öÈßÓàACK£¬ÉèÖÃÎªÓµÈû±ÜÃâ
		//		{
		//			temp2->Threshold = temp2->cwnd / 2;
		//			temp2->cwnd = temp2->Threshold;
		//		}
		//		else      //ÊÕµ½Ç°ÃæÎ´È·ÈÏÊý¾ÝµÄACK
		//		{
		//			temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
		//		}
		//	}
		//	ACK_global = 0;
		//}


	}
}

void mescopy(struct tcp_message tcp_msg_a, struct tcp_message tcp_msg_b)
{
	tcp_msg_b.tcp_src_port = tcp_msg_b.tcp_src_port;
	tcp_msg_b.tcp_dst_port = tcp_msg_b.tcp_dst_port;
	tcp_msg_b.tcp_seq_number = tcp_msg_b.tcp_seq_number;
	tcp_msg_b.tcp_ack_number = tcp_msg_b.tcp_ack_number;
	tcp_msg_b.tcp_hdr_length = tcp_msg_b.tcp_hdr_length;
	tcp_msg_b.tcp_reserved = tcp_msg_b.tcp_reserved;
	tcp_msg_b.tcp_urg = tcp_msg_b.tcp_urg;
	tcp_msg_b.tcp_ack = tcp_msg_b.tcp_ack;
	tcp_msg_b.tcp_psh = tcp_msg_b.tcp_psh;
	tcp_msg_b.tcp_rst = tcp_msg_b.tcp_rst;
	tcp_msg_b.tcp_syn = tcp_msg_b.tcp_syn;
	tcp_msg_b.tcp_fin = tcp_msg_b.tcp_fin;
	tcp_msg_b.tcp_rcv_window = tcp_msg_b.tcp_rcv_window;
	tcp_msg_b.tcp_checksum = tcp_msg_b.tcp_checksum;
	tcp_msg_b.tcp_urg_ptr = tcp_msg_b.tcp_urg_ptr;
	memcpy(tcp_msg_b.tcp_opts_and_app_data, tcp_msg_a.tcp_opts_and_app_data, sizeof(tcp_msg_a.tcp_opts_and_app_data));
}

extern ACK_ID; //当前ACK ID；
extern Count_ACK;//冗余ACK计数器；
extern Rcv_Window; //receiver window //将rwnd加入报文
extern RcvBuffer//可以接受的大小
void Fastretransmit(Receive_ACK_ID)
{
	int count;
	if(Receive_ACK_New!= ACK_ID) //New ID
	{
		Count = 0;
		ACK_ID = Receive_ACK_ID;
		return ;
	}
	else
	{
		{
			Count +1 = Count;
			if(count == 3)
			{
				//开始冗余ACK
			}
		}
	}
	return ;
}

int Getrwnd(int RcvBuffer) //rwdnSize为bufferSize?
{
	int Rcv_Window;
	Rcv_Window = RcvBuffer;
}

void FlowControl(Rcv_Window,Last_Rcv_ACK) //滑动窗口流量控制
{	

	if(Last_Rcv_ACK+Rcv_Window>RcvBuffer)
	{
		//告诉对方已经发满
	}
	elseif(Last_Rcv_ACK+Rcv_Window==RcvBuffer)
	{
		//糊涂窗口综合征
	}

}
