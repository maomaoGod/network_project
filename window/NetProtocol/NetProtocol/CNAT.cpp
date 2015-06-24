#include"stdafx.h"
#include"CNAT.h"

extern int Routing_select;
extern int end_connect;

CNAT::CNAT()
{
	p = NULL;
	initial_IP = oIP;
}

CNAT::~CNAT()
{
}


In_NAT* CNAT::NAT(In_NAT* Nat)
{
	NAT_translation_table* p1 = new NAT_translation_table;
	NAT_translation_table* p2 = new NAT_translation_table;
	if (Nat->DIP == initial_IP)               ///<  外网访问内网，根据端口号改写目的IP和目的端口
	{
		p1 = p;
		while (p1 != NULL)
		{
			if (p1->WAN_port == Nat->Dport)
			{
				Nat->DIP = p1->LAN_IP;
				Nat->Dport = p1->LAN_port;
				break;
			}
			p1 = p1->next_p;
		}
	}
	else                      ///<  内网访问外网，改写源IP和源端口，并添加NAT转换表项
	{
		p1 = p;
		if (p != NULL)
		{
			while (p1->next_p != NULL){
				if ((p1->LAN_IP == Nat->SIP) && (p1->LAN_port == Nat->Sport)) ///< 已存在表项，不添加
				{
					Nat->Sport = p1->WAN_port;
					break;
				}
				p1 = p1->next_p;
			}
			if (p1->next_p == NULL){
				if ((p1->LAN_IP == Nat->SIP) && (p1->LAN_port == Nat->Sport))
					Nat->Sport = p1->WAN_port;
				else{
					p2 = (NAT_translation_table*)malloc(sizeof(NAT_translation_table*));
					p2->WAN_IP = initial_IP;
					p2->WAN_port = p1->WAN_port + 1;
					p2->LAN_IP = Nat->SIP;
					p2->LAN_port = Nat->Sport;
					p2->next_p = NULL;
					p1->next_p = p2;
					Nat->Sport = p2->WAN_port;
				}
			}
		}
		else
		{
			p2 = (NAT_translation_table*)malloc(sizeof(NAT_translation_table*));
			p2->WAN_IP = initial_IP;
			p2->WAN_port = 80 + 1;
			p2->LAN_IP = Nat->SIP;
			p2->LAN_port = Nat->Sport;
			p2->next_p = NULL;
			p = p2;
			Nat->Sport = p->WAN_port;
		}
		Nat->SIP = initial_IP;
	}
	return Nat;
}



