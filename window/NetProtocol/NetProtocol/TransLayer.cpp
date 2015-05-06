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
		printf("the node you want to delete doesn't exist��");
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
{//�ڴ�ͷ���ĵ�����head�в��ҵ�i����㣬���ҵ���0��i��n����
	//�򷵻ظý��Ĵ洢λ�ã����򷵻�NULL��
	tcplist *p;
	p = head;//��ͷ��㿪ʼɨ��
	while (p){//˳ָ�����ɨ�裬ֱ��p->nextΪNULLΪֹ
		if (p->IP = ip)  //���ҵ�Ŀ��IP���򷵻�p
		{
			return p;
		}
		p = p->next;
	}
	return NULL;
}

void TCP_controller()
{
//	// ���߳��ܿص�����
	for (;;)
	{
//		if (New_TCP_Link_Created)
//		{
//			// ���ڳ�ʼ��
//            createNodeList();
//		}
//		foreach (TCP_Entity in TCP_Active_List)
//		{
//			Update(TCP_Entity.State);
//			// ӵ�����Ƹ��´��ڴ�С
            //��Ҫ���͵ı������no_����ip
	    unsigned int ip;
		int no_;
		if (true/*Ҫ���ͱ���*/)
		{
			tcplist* temp1;
			temp1 = GetNode(ip);  //�����ĵ�Դip(+�˿ں�)
			if (temp1 == NULL)
			{
				tcplist* node1 = (tcplist*)malloc(sizeof(tcp_list));
				head->MSG_num = 1;
				head->cwnd = MSS;
				head->IP = ip;
				head->count = 0;
				head->Threshold = 65 * 1024;
				head->tcp_msg[head->MSG_num - 1].ACK = 0;
				head->tcp_msg[head->MSG_num - 1].tcpmessage = global_new_tcp_msg;
				head->tcp_msg[head->MSG_num - 1].time = GetTickCount();
				head->next = NULL;
				addNode(node1);
			}
			else
			{
				if (head->MSG_num - head->count <= head->cwnd / MSS)
				{
					temp1->MSG_num++;
					head->tcp_msg[temp1->MSG_num-1].ACK = 0;
					head->tcp_msg[temp1->MSG_num - 1].tcpmessage = global_new_tcp_msg;
					head->tcp_msg[temp1->MSG_num-1].time = GetTickCount();
				}
				else //wait();
					;
			}
		}
//			Update(TCP_Entity.Window);
//            
//			// ͳ�Ƹ������Ƿ�ack
//			foreach (Msg_Entity in TCP_Entity.Msg_List)
//			{
//				Update(Msg_Entity.ACK_Cnt);
//				Update(Msg_Entity.State);
//				// ����3��ack��������
//				Update(TCP_Entity.Window);
		//ÿ��һ��ʱ��û�յ������ı��ģ��ط�ACK
//		        ACK_global = Msg_Entity.confirm_no;
//			}
//		}
		tcplist* temp3 = head;
		while (temp3)         //ʵʱ���ÿ��TCP�µ�ǰ������Ӧ�ı����Ƿ�ʱδ��Ӧ
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
			//�õ���Ӧ���ĵ�Ŀ��ip(+�˿ں�)
			tcplist* temp2;
			temp2 = GetNode(ip);
			temp2->tcp_msg[temp2->count].time = GetTickCount();
			if (temp2->tcp_msg[temp2->count].tcpmessage.tcp_seq_number <= ACK_global)   //����ACK����
			{
				temp2->tcp_msg[temp2->count].ACK++;
			}
			else
			{
				temp2->count++;
			}
			if (temp2->cwnd <= temp2->Threshold) //������
			{
				temp2->cwnd += MSS;
			}
			else
			{
				if (temp2->tcp_msg[temp2->count].ACK >= 3)    //�յ�3������ACK������Ϊӵ������
				{
					temp2->Threshold = temp2->cwnd / 2;
					temp2->cwnd = temp2->Threshold;
				}
				else      //�յ�ǰ��δȷ�����ݵ�ACK
				{
					temp2->cwnd = temp2->cwnd + MSS*(MSS / temp2->cwnd);
				}
			}
			ACK_global = 0;
		}


		if (true/* TCP_Link_Destroyed */)
		{
//			// �������а���
		    //������Ҫ�����TCP��ip
		    deletenode( GetNode(ip) );
		}
	}
}