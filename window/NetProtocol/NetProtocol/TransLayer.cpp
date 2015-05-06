#include "TransLayer.h"

struct tcplist tcp_list;
tcplist* head = NULL;

extern struct tcp_message global_new_tcp_msg;

int ACK_global;

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
	s1 = head;
	while (s1 != p && s1 != NULL)
	{
		s2 = s1;
		s1 = s1->next;
	}
	if (s1 == NULL)
	{
		printf("the node you want to delete doesn't exist！");
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

tcplist* GetNode(unsigned int ip)
{//在带头结点的单链表head中查找第i个结点，若找到（0≤i≤n），
	//则返回该结点的存储位置，否则返回NULL。
	tcplist *p;
	p = head;//从头结点开始扫描
	while (p){//顺指针向后扫描，直到p->next为NULL为止
		if (p->IP = ip)  //若找到目标IP，则返回p
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

void TCP_controller()
{
	// 单线程总控的流程
	createNodeList();
	for (;;)
	{
	    unsigned int global_ip;
		int global_no_;
		if (true/*要求发送报文*/)
		{
			tcplist* temp1;
			temp1 = GetNode(global_ip);  //请求报文的源ip(+端口号)
			if (temp1 == NULL)   //如果请求报文的源ip对应的TCP当前未建立连接，则新建一个TCP，加入链表尾部
			{
				tcplist* node1 = (tcplist*)malloc(sizeof(tcp_list));
				node1->MSG_num = 1;
				node1->cwnd = MSS;
				node1->IP = global_ip;
				node1->count = 0;
				node1->Threshold = 65 * 1024;
				node1->tcp_msg[node1->MSG_num - 1].ACK = 0;
				node1->tcp_msg[node1->MSG_num - 1].tcpmessage = global_new_tcp_msg;
				node1->tcp_msg[node1->MSG_num - 1].time = GetTickCount();
				node1->next = NULL;
				addNode(node1);
			}
			else     //如果请求报文的源IP对应的TCP端口已建立连接，则根据报文内容，填写当前TCP端口的tcp_msg结构（记录报文相关）
			{
				if (temp1->MSG_num - temp1->count <= temp1->cwnd / MSS)
				{
					temp1->MSG_num++;
					temp1->tcp_msg[temp1->MSG_num-1].ACK = 0;
					temp1->tcp_msg[temp1->MSG_num - 1].tcpmessage = global_new_tcp_msg;
					temp1->tcp_msg[temp1->MSG_num-1].time = GetTickCount();
				}
			}
		}
		tcplist* temp3 = head;
		while (temp3)         //实时检查每个TCP下当前正待响应的报文是否超时未响应
		{
			if (GetTickCount() - temp3->tcp_msg[temp3->count].time > RTT)
			{
				temp3->Threshold = temp3->cwnd / 2;
				temp3->cwnd = MSS;
				temp3->tcp_msg[temp3->count].time = GetTickCount();
			}
			temp3 = temp3->next;
		}
		if (ACK_global != 0)
		{
			//得到响应报文的目标ip(+端口号)
			tcplist* temp2;
			temp2 = GetNode(global_ip);
			temp2->tcp_msg[temp2->count].time = GetTickCount();
			if (temp2->tcp_msg[temp2->count].tcpmessage.tcp_seq_number >= ACK_global)   //冗余ACK计数
			{
				temp2->tcp_msg[temp2->count].ACK++;
			}
			else
			{
				temp2->count++;
			}
			if (temp2->cwnd <= temp2->Threshold) //慢启动
			{
				temp2->cwnd += MSS;
			}
			else
			{
				if (temp2->tcp_msg[temp2->count].ACK >= 3)    //收到3个冗余ACK，设置为拥塞避免
				{
					temp2->Threshold = temp2->cwnd / 2;
					temp2->cwnd = temp2->Threshold;
				}
				else      //收到前面未确认数据的ACK
				{
					temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
				}
			}
			ACK_global = 0;
		}


		if (true/* TCP_Link_Destroyed */)
		{
		    deletenode(GetNode(global_ip));
		}
	}
}