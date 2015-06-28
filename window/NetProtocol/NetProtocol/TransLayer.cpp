#pragma once
#include "stdafx.h"
#include "TransLayer.h"

struct tcplist* head = NULL;

int ACK_global;	// ???
int RTT;		// ???

static float EstimatedRTT;
static float DevRTT;
static int CountACK = 0;	//冗余ACK计数器；???
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

bool global_SR_send_flag;		//SR发送信号
bool global_SR_receive_flag;	//SR接收信号
bool global_SR_send2IP_flag;	//SR目标IP发送信号

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

void TCP_new(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port, int status)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
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
		// 未建立TCP连接
		printf("TCP link has not been established!\n");
		return;
	}
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_send_flag);
	global_send_tcp = found_tcp;
	global_send_sockstruct = data_from_applayer;
	global_TCP_send_flag = true;
	while (global_TCP_send_flag);
}

void TCP_receive(struct Msg data_from_netlayer)
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_receive_flag);
	global_receive_ip_msg = data_from_netlayer;
	global_TCP_receive_flag = true;
	while (global_TCP_receive_flag);
}

void TCP_resend()
{
	// 轮询，因为网速快于处理速度，轮询效率反而高
	while (global_TCP_resend_flag);
	global_TCP_resend_flag = true;
	while (global_TCP_resend_flag);
}

void TCP_close(unsigned int src_ip, unsigned short src_port, unsigned int dst_ip, unsigned short dst_port)
{
	struct tcplist *found_tcp = getNode(src_ip, src_port, dst_ip, dst_port);
	if (found_tcp == NULL)
	{
		// 未建立TCP连接，静默
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
	// 单线程总控的流程
	// 初始化随机器
	srand(time(0));
	// 初始化TCP连接链表
	createNodeList();
	// 初始化TCP操作标志
	global_TCP_new_flag = global_TCP_send_flag = global_TCP_resend_flag = global_TCP_receive_flag = global_TCP_close_flag = false;
	// 初始化监听标志
	memset(listening_flag, 0, sizeof(listening_flag));

	// 控制流
	for (;;)
	{
		// 是否需要新建一个TCP连接
		if (global_TCP_new_flag)
		{
			struct tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
			// 内存耗尽
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

		// 是否需要发送一个TCP报文，这里并不是真正的发送，而是加入待发送报文队列
		// 等待合适时机向下递交
ctrl_send:
		if (global_TCP_send_flag)
		{
			// 处理数据
			//unsigned int src_ip = getIP();
			//unsigned short src_port = global_send_sockstruct.srcport;
			//unsigned int dst_ip = IP_chars2uint(global_send_sockstruct.dstip);
			//unsigned short dst_port = global_send_sockstruct.dstport;	

			// 根据四元组找到TCP连接链表中的对应表
			struct tcplist *tcp = global_send_tcp;
			
			// 发送缓冲区耗尽
			int unack_size = tcp->wait_for_fill-tcp->wait_for_ack;
			if (unack_size+global_send_sockstruct.datalength > SEND_BUFFER_SIZE)
			{
				printf("Out of send-buffer! Cannot send these data!\n");
				global_TCP_send_flag = false;
				goto ctrl_receive;
			}

			// 将要发数据填入发送缓冲
			for (int i = 0; i < global_send_sockstruct.datalength; ++i)
			{
				tcp->tcp_buf_send[(i+tcp->wait_for_fill)%SEND_BUFFER_SIZE] = global_send_sockstruct.data[i];
			}
			// 更新待填充序号
			tcp->wait_for_fill = tcp->wait_for_fill+global_send_sockstruct.datalength;
			global_TCP_send_flag = false;
		}

		// 是否需要接收一个TCP报文，将网络层交付的报文填入接收缓存，等待合适
		// 时机向上交付
ctrl_receive:
		if (global_TCP_receive_flag)
		{
			// 获取TCP报文段
			struct tcp_message new_tcp_msg;
			memcpy(&new_tcp_msg, global_receive_ip_msg.data, global_receive_ip_msg.datelen);

			// opts和data一同进行检验
			unsigned opts_data_len = global_receive_ip_msg.datelen-20;

			////////// 检验和
			////////if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			////////{
			////////	// 舍弃报文
			////////	global_TCP_receive_flag = false;
			////////	goto ctrl_close;
			////////}
			
			// 处理数据，需要反过来！！！
			unsigned int dst_ip = global_receive_ip_msg.sip;
			unsigned short dst_port = new_tcp_msg.tcp_src_port;
			unsigned int src_ip = global_receive_ip_msg.dip;
			unsigned short src_port = new_tcp_msg.tcp_dst_port;

			// 根据四元组找到TCP连接链表中的对应表
			struct tcplist *tcp = getNode(src_ip, src_port, dst_ip, dst_port);

			// 被第一次握手
			if (new_tcp_msg.tcp_syn == 1 && new_tcp_msg.tcp_ack == 0)
			{
				if (!check_listening(src_port))
				{
					// 未监听，静默过滤
					global_TCP_receive_flag = false;
					goto ctrl_close;
				}

				struct tcplist *new_tcp = (tcplist *)malloc(sizeof(struct tcplist));
			
				// 内存耗尽
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

				// 更新对方的seq_number系列
				new_tcp->send_ack_needed = true;
				new_tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
				new_tcp->last_read = new_tcp->last_rcvd = new_tcp_msg.tcp_seq_number;
			}

			// 被第二次握手
			else if (new_tcp_msg.tcp_syn == 1 && new_tcp_msg.tcp_ack == 1)
			{
				if (tcp == NULL)
				{
					// 伪造的第二次握手
					printf("Under attack of fake shaking message!\n");
					global_TCP_receive_flag = false;
					goto ctrl_close;
				}
				else
				{
					// 确实在等待第二次握手
					if (tcp->connect_status == LINK_WAIT_FOR_SYN)
					{
						tcp->connect_status = LINK_GOT_SYN;

						// 更新对方的seq_number系列
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						tcp->last_read = tcp->last_rcvd = new_tcp_msg.tcp_seq_number;
					}
					// 不在等待第二次握手，静默（或其他处理方案？）
					else
					{
						// do nothing
					}
				}
			}

			// 被第三次握手
			else if (tcp != NULL && tcp->connect_status == LINK_WAIT_FOR_SYNACK && new_tcp_msg.tcp_ack == 1)
			{
				// 确实在等待第三次握手
				tcp->connect_status = LINK_GOT_SYNACK;

				// 更新对方的seq_number系列
				tcp->send_ack_needed = false;
				++(tcp->last_read);
				++(tcp->last_rcvd);
			}

			// 收到FIN
			else if (new_tcp_msg.tcp_fin == 1 && new_tcp_msg.tcp_ack == 0)
			{
				if (tcp == NULL)
				{
					// 伪造的断开连接，静默过滤
				}
				else
				{
					// 确实可以被FIN，本方未半开
					if (tcp->connect_status == LINK_CONNECT_BIDIR)
					{
						// 状态变为本方半开
						tcp->connect_status = LINK_FINISHED;

						// 更新对方的seq_number系列
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						// 如果有数据未确认就不可以发送FIN，所以可以直接表示read过了
						++(tcp->last_read);
						++(tcp->last_rcvd);
					}
					// 确实在可以FIN，本方已半开
					else if (tcp->connect_status == LINK_SELF_HALF_OPEN)
					{
						// 状态变为连接丢失
						tcp->connect_status = LINK_CONNECT_DESTROYED;

						// 更新对方的seq_number系列
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number+1;
						// 如果有数据未确认就不可以发送FIN，所以可以直接表示read过了
						++(tcp->last_read);
						++(tcp->last_rcvd);
					}
					// 不在等待FINACK，静默（或其他处理方案？）
					else
					{
						// do nothing
					}
				}
			}

			// 收到FINACK
			else if (new_tcp_msg.tcp_fin == 1 && new_tcp_msg.tcp_ack == 1)
			{
				if (tcp == NULL)
				{
					// 伪造的断开连接，静默过滤
				}
				else
				{
					// 确实在等待FINACK，对方未半开
					if (tcp->connect_status == LINK_WAIT_FOR_FINACK)
					{
						// 状态变为本方半开
						tcp->connect_status = LINK_SELF_HALF_OPEN;
					}
					// 确实在等待FINACK，对方已半开
					else if (tcp->connect_status == LINK_WAIT_FOR_DESACK)
					{
						// 状态变为连接丢失
						tcp->connect_status = LINK_CONNECT_LOSE;
					}
					// 不在等待FINACK，静默（或其他处理方案？）
					else
					{
						// do nothing
					}
				}
			}
			
			// 不是TCP控制信息，是一般报文
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
					// 可能是之前未收到的报文，或者是重复收到的报文
					if (rcvd_msg_existed(tcp, new_tcp_msg.tcp_seq_number))
					{
						// 是重复收到的报文，舍弃
						global_TCP_receive_flag = false;
						goto ctrl_close;
					}
					else
					{
						// 是之前未收到的报文
						if (tcp->last_read+1 == new_tcp_msg.tcp_seq_number)
						{
							// 恰好是我们之前等待的，接收
							// 将所接收数据填入接收缓冲，不包括opts
							for (int i = 0; i < data_len; ++i)
							{
								tcp->tcp_buf_rcvd[(i+tcp->last_read+1)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
							}
							// 无需更新已填充的最后一个字节流的序号

							// 加入接收报文队列
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = tcp->last_read+1;
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;

							// 更新ack
							tcp->send_ack_needed = true;
							tcp->next_send_ack = new_tcp_msg.tcp_seq_number+data_len;
						}
						else
						{
							// 不是我们之前等待的，但是也能填补空隙
							// 将所接收数据填入接收缓冲，不包括opts
							for (int i = 0; i < data_len; ++i)
							{
								tcp->tcp_buf_rcvd[(i+new_tcp_msg.tcp_seq_number)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
							}
							// 无需更新已填充的最后一个字节流的序号

							// 加入接收报文队列
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = new_tcp_msg.tcp_seq_number;
							tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;

							// 更新ack
							tcp->send_ack_needed = true;
							tcp->next_send_ack = tcp->last_read+1;
						}
					}
				}
				else
				{
					// 可能是正在期待的报文，也可能是跳步报文，需要缓存而不能交付
					if (tcp->last_rcvd+1 == new_tcp_msg.tcp_seq_number)
					{
						// 接收缓冲区耗尽（由于流量控制，一般不可能发生）
						int unhandin_size = tcp->last_rcvd-tcp->last_read;
						if (unhandin_size+global_receive_ip_msg.datelen > RCVD_BUFFER_SIZE)
						{
							printf("Out of receive-buffer! Cannot store these data!\n");
							global_TCP_receive_flag = false;
							goto ctrl_close;
						}

						// 恰好是我们之前等待的，接收
						// 将所接收数据填入接收缓冲，不包括opts
						for (int i = 0; i < data_len; ++i)
						{
							tcp->tcp_buf_rcvd[(i+tcp->last_rcvd+1)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
						}
						// 需更新已填充的最后一个字节流的序号
						tcp->last_rcvd = tcp->last_rcvd+data_len;

						// 加入接收报文队列
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = tcp->last_rcvd+1;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;

						// 更新ack
						tcp->send_ack_needed = true;
						tcp->next_send_ack = new_tcp_msg.tcp_seq_number + data_len;
					}
					else
					{
						// 接收缓冲区耗尽（由于流量控制，一般不可能发生）
						int unhandin_size = new_tcp_msg.tcp_seq_number-1-tcp->last_read;
						if (unhandin_size+global_receive_ip_msg.datelen > RCVD_BUFFER_SIZE)
						{
							printf("Out of receive-buffer! Cannot store these data!\n");
							global_TCP_receive_flag = false;
							goto ctrl_close;
						}

						// 不是我们之前等待的，是跳步报文
						// 将所接收数据填入接收缓冲，不包括opts
						for (int i = 0; i < data_len; ++i)
						{
							tcp->tcp_buf_rcvd[(i+new_tcp_msg.tcp_seq_number)%RCVD_BUFFER_SIZE] = new_tcp_msg.tcp_opts_and_app_data[i+opts_offset];
						}
						// 需更新已填充的最后一个字节流的序号
						tcp->last_rcvd = new_tcp_msg.tcp_seq_number+data_len;

						// 加入接收报文队列
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].datalen = data_len;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].seq_number = new_tcp_msg.tcp_seq_number;
						tcp->tcp_msg_rcvd[tcp->last_rcvd_msg].handin = false;

						// 更新ack
						tcp->send_ack_needed = true;
						tcp->next_send_ack = tcp->last_read+1;
					}
				}

				// 更新对方的接收窗口
				tcp->rcvd_wind = new_tcp_msg.tcp_rcv_window;

ctrl_ack:
				// ack字段是否有效
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
					// ack有效，判断是否是冗余或确认ack
					if (new_tcp_msg.tcp_ack_number >= tcp->wait_for_ack)
					{
						// 累计确认ack
						tcp->wait_for_ack = new_tcp_msg.tcp_ack_number;
						//// 继续向后确认，防止该次ack是填补空隙（涉嫌混淆收发）
						//tcp->wait_for_ack = next_ack_place(tcp, tcp->wait_for_ack);
					}
					else
					{
						// 冗余ack
						if (new_tcp_msg.tcp_ack_number == tcp->last_rcvd_ack)
						{
							++(tcp->ack_count);
							if (tcp->ack_count >= 3)
							{
								// 3次冗余ack，快速重传
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

				// 发送ACK，以后会改
				//Temp_Send_ACK(tcp); Dead Lock!!!!
			}

			// 有什么用？ERROR....EDITING Needed
			// 计算RTT
			//RTT = (int)getSampleRTT(tcp->tcp_msg_send[tcp->wait_for_ack_msg].time, GetTickCount());

			global_TCP_receive_flag = false;
		}

		// 重传..有点问题？
		if (global_TCP_resend_flag)
		{
		}

		// 关闭TCP连接，如果双关闭则拆除
ctrl_close:
		if (global_TCP_close_flag)
		{
			// 双方都关闭就拆除TCP连接
			if (global_close_tcp->connect_status = LINK_PEER_HALF_OPEN)
			{
				// 对方已经关闭
				global_close_tcp->connect_status = LINK_CONNECT_DESTROYING;
			}
			else
			{
				// 关闭本方的tcp连接，但是仍然可以接收到对方的数据，可以发ack
				global_close_tcp->connect_status = LINK_FINISHING;
			}
		}

		//tcplist* single_tcp = head;
		//while (single_tcp)         //实时检查每个TCP下当前正待响应的报文是否超时未响应
		//{
		//	if (GetTickCount() - single_tcp->tcp_msg_send[single_tcp->wait_for_ack_msg].time > RTT)
		//	{
		//		single_tcp->Threshold = single_tcp->cwnd / 2;
		//		single_tcp->cwnd = MSS;
		//		single_tcp->tcp_msg_send[single_tcp->wait_for_ack_msg].time = GetTickCount();
		//		//sendtoip(single_tcp->tcp_msg_send[single_tcp->MSG_ACK].tcpmessage, single_tcp->IP, 第一个参数这个报文的长度);
		//	}
		//	single_tcp = single_tcp->next;
		//}

		// 遍历tcp管理链表，处理待处理的信息
		struct tcplist *single_tcp = head->next;	// fuck your mom
		while (single_tcp != NULL)
		{
			if (single_tcp->connect_status == LINK_CONNECT_BIDIR || single_tcp->connect_status == LINK_PEER_HALF_OPEN)
			{
				// 向下递交报文段
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
							
					// 送往网络层
					TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, single_tcp->tcp_msg_send[single_tcp->wait_for_fill_msg].datalen);

					single_tcp->wait_for_send = new_send;
					single_tcp->wait_for_fill_msg++;
					single_tcp->send_ack_needed = false;
				}

				if (single_tcp->send_ack_needed)
				{
					Temp_Send_ACK(single_tcp);
				}

				// 向上交付报文
				if (single_tcp->last_rcvd > single_tcp->last_read)
				{
					// 填入送往应用层的结构中
					struct sockstruct *new_sockstruct = new sockstruct();
					new_sockstruct->dstport = single_tcp->tcp_src_port;
					new_sockstruct->srcport = single_tcp->tcp_dst_port;
					new_sockstruct->funcID = SOCKSEND;
					new_sockstruct->datalength = single_tcp->last_rcvd - single_tcp->last_read;
					IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
					IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
					// ---------由socket释放，这个写法似乎不太规范--------------------------------
					new_sockstruct->data = (char *)malloc(new_sockstruct->datalength*sizeof(char));
					// ---------由socket释放，这个写法似乎不太规范--------------------------------
					memcpy(new_sockstruct->data, single_tcp->tcp_buf_rcvd+single_tcp->last_read+1, new_sockstruct->datalength);

					// 送往应用层
					AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);

					single_tcp->last_read = single_tcp->last_rcvd;
					// 标记已经上交（这个似乎不需要？看样子可以把handin字段删掉）
					// 甚至整个tcpmsg_rcvd都没球的用吧！！！
				}
			}

			// 发送SYN，第一次握手
			else if (single_tcp->connect_status == LINK_CONNECTING)
			{
				// 构造SYN报文段
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
			
				// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// 改变tcp连接的状态，等待对方发来的syn报文
				single_tcp->connect_status = LINK_WAIT_FOR_SYN;

				// 更新缓冲区指针，装作从TCP_send发过数据的状态
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
			}

			// 发送SYN和ACK，第二次握手
			else if (single_tcp->connect_status == LINK_CONNECTED)
			{
				// 构造ACK&SYN报文段
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
			
				// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// 改变tcp连接的状态，等待对方发来的synack报文
				single_tcp->connect_status = LINK_WAIT_FOR_SYNACK;

				// 更新缓冲区指针，装作从TCP_send发过数据的状态
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				// 通知应用层Accept事件
				// 填入送往应用层的结构中
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
				new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
				new_sockstruct->funcID = SOCKACCEPT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// 送往应用层
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// 发送SYNACK，第三次握手
			else if (single_tcp->connect_status == LINK_GOT_SYN)
			{
				// 构造SYNACK报文段
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
			
				// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// 改变tcp连接的状态，已连接
				single_tcp->connect_status = LINK_CONNECT_BIDIR;

				// 更新缓冲区指针，装作从TCP_send发过数据的状态
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				// 通知应用层可以分配资源了
				// 填入送往应用层的结构中
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
				new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
				new_sockstruct->funcID = SOCKCONNECT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// 送往应用层
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// 通知应用层分配资源
			else if (single_tcp->connect_status == LINK_GOT_SYNACK)
			{
				// 改变tcp连接的状态，已连接
				single_tcp->connect_status = LINK_CONNECT_BIDIR;

				// 填入送往应用层的结构中
				struct sockstruct *new_sockstruct=new sockstruct();
				new_sockstruct->dstport = single_tcp->tcp_src_port;
				new_sockstruct->srcport = single_tcp->tcp_dst_port;
				new_sockstruct->funcID = SOCKCONNECT;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// 送往应用层
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);
			}

			// 关闭本方连接，发送FIN
			else if (single_tcp->connect_status == LINK_FINISHING
					|| single_tcp->connect_status == LINK_CONNECT_DESTROYING)
			{
				// 在发完数据并得到所有数据的确认之前不可以发送FIN
				if (single_tcp->wait_for_ack == single_tcp->wait_for_fill)
				{
					// 构造FIN报文段
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
			
					// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录fin
					TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

					// 更新缓冲区指针，装作从TCP_send发过数据的状态
					++(single_tcp->seq_number);
					++(single_tcp->wait_for_fill);
					++(single_tcp->wait_for_ack);
					++(single_tcp->wait_for_send);

					if (single_tcp->connect_status == LINK_FINISHING)
					{
						// 改变tcp连接的状态，等待对方发来的finack报文
						single_tcp->connect_status = LINK_WAIT_FOR_FINACK;
					}
					else //if (single_tcp->connect_status == LINK_CONNECT_DESTROYING)
					{
						// 改变tcp连接的状态，等待对方发来的finack报文
						single_tcp->connect_status = LINK_WAIT_FOR_DESACK; 
					}
				}
			}

			// 发送FIN的ACK
			else if (single_tcp->connect_status == LINK_FINISHED
					|| single_tcp->connect_status == LINK_CONNECT_DESTROYED)
			{
				// 构造ACK报文段
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
			
				// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录SYN
				TCP_Send2IP(new_tcp_msg, single_tcp->tcp_src_ip, single_tcp->tcp_dst_ip, 1);

				// 更新缓冲区指针，装作从TCP_send发过数据的状态
				++(single_tcp->seq_number);
				++(single_tcp->wait_for_fill);
				++(single_tcp->wait_for_ack);
				++(single_tcp->wait_for_send);
				single_tcp->send_ack_needed = false;

				if (single_tcp->connect_status == LINK_FINISHED)
				{
					// 改变tcp连接的状态，变为对方半开状态
					single_tcp->connect_status = LINK_PEER_HALF_OPEN;
				}
				else if (single_tcp->connect_status == LINK_CONNECT_DESTROYED)
				{
					// 改变tcp连接的状态，变为双向断开
					single_tcp->connect_status = LINK_CONNECT_LOSE;
				}
			}
			else
			{
				// nothing
			}

			// 断开连接，一定要用if咯，不能接着用elseif
			if (single_tcp->connect_status == LINK_CONNECT_LOSE)
			{
				// 填入送往应用层的结构中
				struct sockstruct *new_sockstruct = new sockstruct();
				new_sockstruct->dstport = single_tcp->tcp_src_port;
				new_sockstruct->srcport = single_tcp->tcp_dst_port;
				new_sockstruct->funcID = SOCKCLOSE;
				new_sockstruct->datalength = 0;
				IP_uint2chars(new_sockstruct->srcip, single_tcp->tcp_dst_ip);
				IP_uint2chars(new_sockstruct->dstip, single_tcp->tcp_src_ip);
				new_sockstruct->data = NULL;

				// 送往应用层
				AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);

				// 销毁TCP连接
				deleteNode(single_tcp);
			}

			single_tcp = single_tcp->next;
		}
		//free(single_tcp);杀妈之作
	}
}

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

//int Fastretransmit(int ACK_global)    //返回需要重发的ACK序号
//{
//	int retransmitACKID;
//	retransmitACKID = ACK_global;
//	return retransmitACKID;
//}

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

void initialRTT()                   //初始化RTT需要的变量
{
	EstimatedRTT = 0;
	DevRTT = 0;
}

float getSampleRTT(int sendtime, int gettime)		//往返时延的估计与超时，返回超时时间
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

	// 发往网络层
	AfxGetApp()->m_pMainWnd->PostMessage(IPTOLINK, (WPARAM)new_ip_msg);
}

void UDP_Send2IP(struct sockstruct data_from_applayer, unsigned int src_ip, unsigned int dst_ip, unsigned int data_len)
{
	unsigned short dst_port = data_from_applayer.dstport;
	unsigned short src_port = data_from_applayer.srcport;

	struct udp_message new_udp_msg;
	// 填入UDP报文段结构
	new_udp_msg.udp_src_port = src_port;
	new_udp_msg.udp_dst_port = dst_port;
	new_udp_msg.udp_msg_length = 8 + data_len;
	memcpy(new_udp_msg.udp_app_data, data_from_applayer.data, data_len + 1); // +1 for \0
	new_udp_msg.udp_checksum = udpmakesum((u16)data_len, (u16)src_port, (u16)dst_port, data_len % 2, (u16 *)&(new_udp_msg.udp_app_data));

	// UDP无拥塞控制
	struct Msg *new_ip_msg = new Msg();
	new_ip_msg->sip = src_ip;
	new_ip_msg->dip = dst_ip;
	new_ip_msg->ih_sport = src_port;
	new_ip_msg->ih_dport = dst_port;
	new_ip_msg->datelen = new_udp_msg.udp_msg_length;
	memcpy(new_ip_msg->data, &new_udp_msg, new_ip_msg->datelen);
	new_ip_msg->protocol = PROTOCOL_UDP;	// 17 for UDP

	// 发往网络层
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

void stop_wait()   //停止等待
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

		if (global_sandw_send_flag)   //如果收到应用层传下来的报文
		{
			new_sandw.send_buf[new_sandw.last_waitforsend_msg] = global_send_sockstruct_sandw;  //将该报文放入发送缓冲区
			new_sandw.last_waitforsend_msg++;
			global_sandw_send_flag = false;
		}

		if (global_sandw_receive_flag)   //如果收到下层传上来的报文
		{
			struct tcp_message new_tcp_msg;
			memcpy(&new_tcp_msg, global_receive_ip_msg_sandw.data, global_receive_ip_msg_sandw.datelen);

			// opts和data一同进行检验
			unsigned opts_data_len = global_receive_ip_msg_sandw.datelen - 20;

			// 检验和
			if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			{
				// 舍弃报文
				global_sandw_receive_flag = false;
				break;
			}

			if (new_tcp_msg.tcp_ack)   //如果是应答报文ack
			{
				if (new_tcp_msg.tcp_ack_number == new_sandw.last_send.tcp_seq_number)  //如果收到的ACK确认号和发送的报文序号相同
				{
					new_sandw.last_ack = new_tcp_msg.tcp_ack_number;
					global_sandw_sendtoIP_flag = true;

				}
				//如果不是之前发送的那个报文的应答报文，就舍弃，不做处理
			}

			if (new_tcp_msg.tcp_ack == 0 || global_receive_ip_msg_sandw.datelen > 20)  //如果收到的不是一个应答报文，而是别人发过来的一个报文的话，要发一个ack过去
			{
				struct tcp_message new_tcp_message;
				new_tcp_message.tcp_src_port = new_tcp_msg.tcp_dst_port;   //ack的源端口号和目的端口号与收到的报文相反
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

			/*将接收到的报文封装发给上层APP*/
			struct sockstruct *new_sockstruct = new sockstruct();
			new_sockstruct->dstport = new_tcp_msg.tcp_src_port;
			new_sockstruct->srcport = new_tcp_msg.tcp_dst_port;
			new_sockstruct->funcID = SOCKSEND;
			new_sockstruct->datalength = global_receive_ip_msg_sandw.datelen - 20;
			IP_uint2chars(new_sockstruct->srcip, new_tcp_msg.tcp_src_port);
			IP_uint2chars(new_sockstruct->dstip, new_tcp_msg.tcp_dst_port);
			// ---------由socket释放，这个写法似乎不太规范--------------------------------
			new_sockstruct->data = (char *)malloc(new_sockstruct->datalength*sizeof(char));
			// ---------由socket释放，这个写法似乎不太规范--------------------------------
			memcpy(new_sockstruct->data, new_tcp_msg.tcp_opts_and_app_data, new_sockstruct->datalength);

			// 送往应用层	
			AfxGetApp()->m_pMainWnd->PostMessage(APPSEND, (WPARAM)new_sockstruct);



			global_sandw_receive_flag = false;
		}


		if (global_sandw_sendtoIP_flag)    //收到要从发送缓冲区取一个报文发送出去的信号
		{
			if (new_sandw.last_waitforsend_msg > new_sandw.last_send_msg)  //如果缓冲区还有没有发送的报文
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

				//将该报文发送到IP层
				TCP_Send2IP(new_tcp_message, IP_chars2uint(new_sandw.send_buf[new_sandw.last_send_msg].srcip), IP_chars2uint(new_sandw.send_buf[new_sandw.last_send_msg].dstip), datalen);

				new_sandw.last_send = new_tcp_message;    //记录这次发送出去的报文，为之后超时重传做基础
				new_sandw.time = GetTickCount();           //重新记录发送时间
				new_sandw.last_send_msg++;                //发送缓冲区中已经发送的报文下标加一
			}
			global_sandw_sendtoIP_flag = false;
		}

		//超时重传new_sandw.last_send

	}
}



//以下为选择重传，报文交付给上层和传递给下层的部分都没写

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

void _SR() //选择重传
{
	srand(time(0));//超时未考虑
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

			// 检验和
			if (!tcpcheck(opts_data_len, new_tcp_msg.tcp_src_port, new_tcp_msg.tcp_dst_port, opts_data_len % 2, (u16 *)&(new_tcp_msg.tcp_opts_and_app_data), new_tcp_msg.tcp_checksum))
			{
				// 舍弃报文
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
			//交付new_SR.rcv_buf[rcv_wnd]中的报文。。
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
	// 构造ACK报文段
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
			
	// 不走TCP_send()，不加入TCP报文结构和报文缓冲，因为其中未记录ACK
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
			//检查所有tcp上已发送但未确认的那些报文是否超时
			for (temp_msg_num = single_tcp->wait_for_ack_msg; temp_msg_num < single_tcp->wait_for_fill_msg; temp_msg_num++)
			{
				if (GetTickCount() - single_tcp->tcp_msg_send[temp_msg_num].time > RTT)
				{
					//超时拥塞控制
					single_tcp->threshold = single_tcp->cong_wind / 2;
					single_tcp->cong_wind = MSS;
					single_tcp->tcp_msg_send[temp_msg_num].time = GetTickCount();
					//下面为超时重传已发送但未确认的第一个报文
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

					// 送往网络层
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
