#define RoutePROTOCOL_TCP 6
#define RoutePROTOCOL_UDP 17
#define LINKSEND WM_USER+1002
#define ipMSS 1480						///< ����IP���ݱ����ķ�Ƭ��С1480
#define MSS 1024
#define maxnum 100				///<�������Ľڵ���
#define maxint 999999			///<������Ľڵ�����
#define MAX 0x3f3f3f3f
#define N 1010
#define data_info 1



/**
*@class <Edge>
*@author ACM2012
*@note
*	��Ҫ���һ�������Ľṹ��Ȼ��ѱߵĸ�����ÿ���ߵĳ�ʼ�ڵ�u,v,����cost����һ��Edge�ṹ
*/
struct Edge{
	int u, v;
};

/**
*@class <common_data>
*@author ACM2012
*@note
*	����LS�㷨·����Ϣ��DV�㷨·����Ϣ�Ĺ�ͬ�ṹ
*/
struct common_data{
	int node;				 ///< �ڵ����
	int sid;				 ///< Դ�ڵ�
	int did;				 ///< Ŀ�Ľڵ�
	int dist[maxnum];		 ///< ��ʾ��ǰ�㵽Դ������·������
	int pre[maxnum];		 ///< ��¼��ǰ���ǰһ�����
};

/**
*@class <LS_data>
*@author ACM2012
*@note
*	����LS�㷨·����Ϣ�ṹ
*/
struct LS_data{
	common_data LsData;
	int c[maxnum][maxnum];   ///< ��¼ͼ�������·������
};

/**
*@class <DV_data>
*@author ACM2012
*@note
*	����DV�㷨·����Ϣ�ṹ
*/
struct DV_data{
	common_data DvData;
	int edgenum;			 ///< ����
	Edge E[maxnum];			 ///< �߽ṹ����
};


/**
*@class <Route_info>
*@author ACM2012
*@note
*	����·����Ϣ�ṹ
*/
struct Route_info{
	Edge E[maxnum];
	int node;
	int edgenum;
	int sid;
	int did;
};

/**
*@class <IP_Msg>
*@author ACM2012
*@note
*	��������㴫����·��Ľṹ
*/
struct RouteIP_Msg{
	unsigned int ih_saddr;		///< 32λԴIP
	unsigned int ih_daddr;		///< 32λĿ��IP
	unsigned short ih_len;	    ///< 16λ���ݱ��ܳ���
	char *data;
};

/**
*@class UDP���Ķνṹ
*@brief ���UDP���ĶεĽṹ��
*@author ACM1201
*@note
*UDP���Ķλ����ṹ
*/
struct Routeudp_message
{
	unsigned udp_src_port : 16;			///< Դ�˿ں�
	unsigned udp_dst_port : 16;			///< Ŀ�Ķ˿ں�
	unsigned udp_msg_length : 16;		///< ���ĳ���
	unsigned udp_checksum : 16;			///< �����
	char udp_app_data[MSS];			///< ����
};

/**
*@class TCP���Ķνṹ
*@brief ���TCP���ĶεĽṹ��
*@author ACM1201
*@note
*TCP���Ķλ����ṹ
*/
struct Routetcp_message
{
	unsigned tcp_src_port : 16;	///< Դ�˿ں�
	unsigned tcp_dst_port : 16;	///< Ŀ�Ķ˿ں�
	unsigned tcp_seq_number : 32;	///< ���
	unsigned tcp_ack_number : 32;	///< ȷ�Ϻţ����ڴ�����һ����ţ�
	unsigned tcp_hdr_length : 4;	///< �ײ����ȣ�ͨ��Ϊ20Byte��
	unsigned tcp_reserved : 6;	///< �����ֶΣ���δʹ�ã�
	unsigned tcp_urg : 1;	///< ������־����δʹ�ã�
	unsigned tcp_ack : 1;	///< ȷ�ϱ�־
	unsigned tcp_psh : 1;	///< �����Ͻ���־����δʹ�ã�
	unsigned tcp_rst : 1;	///< �������ӽ�����������δʹ�ã�
	unsigned tcp_syn : 1;	///< �������ӽ�������
	unsigned tcp_fin : 1;	///< �������ӽ�������
	unsigned tcp_rcv_window : 16;	///< ���մ���
	unsigned tcp_checksum : 16;	///< �����
	unsigned tcp_urg_ptr : 16;	///< ��������ָ�루��δʹ�ã�
	char tcp_opts_and_app_data[MSS];	///< ѡ���ֶΣ�ͨ��û�У���ͨ���ײ�����Ϊ20Byte��������
};

/**
*@class <Routeip_message>
*@brief ���IP�ײ��Ľṹ��
*@author ACM2012
*@note
*�Լ������IP�����ײ�
*/
struct Routeip_message //����IP���ݱ���
{
	unsigned ih_version : 4;            ///< ��ʶ�����ݰ���IP�汾��,һ��4λ, 0100��ʾIPv4, 0110��ʾIPv6
	unsigned ip_hdr_length : 4;         ///< �ײ�����, һ��Ϊ20�ֽ�
	unsigned ih_sever : 8;              ///< 8bit�ķ�������
	unsigned ih_data_len : 16;	        ///< 16λ���ݱ����ȵ�λΪ�ֽ�

	unsigned ih_ident : 16;	            ///< ���ݱ���16bit��ʶ
	unsigned ih_flags : 3;              ///< ���ݱ���3bit��־
	unsigned ih_offset : 13;	        ///< ���ݱ���13bitƬƫ��

	unsigned ih_TTL : 8;                ///< ���ݱ�������
	unsigned ih_protl : 8;	            ///< ���ݱ���Э��, ����0�����������, ����1����ڵ���ӵ�·����Ϣ
	unsigned ip_checksum : 16;		    ///< ���ݱ����ײ������

	unsigned ih_saddr : 32;		            ///< 32λԴIP
	unsigned ih_daddr : 32;		            ///< 32λĿ��IP
	char data[ipMSS];
	bool operator == (const Routeip_message &it) const
	{
		if (ih_version != it.ih_version) return false;
		if (ip_hdr_length != it.ip_hdr_length) return false;
		if (ih_sever != it.ih_sever) return false;
		if (ih_data_len != it.ih_data_len) return false;
		if (ih_ident != it.ih_ident) return false;
		if (ih_flags != it.ih_flags) return false;
		if (ih_offset != it.ih_offset) return false;
		if (ih_TTL != it.ih_TTL) return false;
		if (ih_protl != it.ih_protl) return false;
		if (ip_checksum != it.ip_checksum) return false;
		if (ih_saddr != it.ih_saddr) return false;
		if (ih_daddr != it.ih_daddr) return false;
		return true;
	}
	bool operator != (const Routeip_message &it) const
	{
		return !((*this) == it);
	}
};

/**
*@class <NAT_translation_table>
*@author ACM2012
*@note
*	����NATת����
*/
struct NAT_translation_table
{
	unsigned int WAN_IP;			 ///<  ����IP
	unsigned short WAN_port;         ///<  �����˿�
	unsigned int LAN_IP;			 ///<  ����IP
	unsigned short LAN_port;		 ///<  �����˿�
	struct NAT_translation_table* next_p;
};

/**
*@class <In_NAT>
*@author ACM2012
*@note
*	����NATЭ����Ҫ������Ϣ�Ľṹ
*/
struct In_NAT{
	unsigned int SIP;			 ///<  ����IP
	unsigned short Sport;        ///<  �����˿�
	unsigned int DIP;			 ///<  ����IP
	unsigned short Dport;        ///<  �����˿�
};


class CRoute
{
public:
	CRoute(void);
public:
	virtual ~CRoute(void);
	virtual void RS(int Routing_Select)=0;
	virtual int Datainfo(WPARAM wparam, LPARAM lparam)=0;
	virtual unsigned int getLocalIP()=0;
	virtual unsigned int getPublicIP()=0;
	virtual In_NAT* NAT(In_NAT* Nat)=0;
	virtual unsigned short iphdrmakesum(Routeip_message *ip)=0;
	virtual int IP2Link(WPARAM wparam, LPARAM lparam)=0;
public:
	virtual int  RecvMsg(WPARAM wparam, LPARAM lparam)=0;
	virtual int  SendMsg(WPARAM wparam, LPARAM lparam)=0;
	virtual unsigned int Num2IP(int num)=0;
	virtual int  IP2Num(unsigned int IP)=0;

	virtual void Route2LS(Route_info *info, LS_data *LsData)=0;
	virtual void Route2DV(Route_info *info, DV_data *DvData)=0;
	virtual int  searchPath(int *pre, int v, int u)=0;
	virtual void Dijkstra(int n, int v, int *dist, int *pre, int c[maxnum][maxnum])=0;
	virtual int  LS(LS_data *LSdata)=0;
	virtual void Bellman_Ford(int n, int edgenum, int v, int *dist, int *pre, Edge edge[N])=0;
	virtual int  DV(DV_data *DVdata)=0;
};