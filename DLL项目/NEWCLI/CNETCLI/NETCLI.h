#include "CNETCLI.h"

extern "C" __declspec(dllexport) CNETCLI* CreateCliPtr();

extern "C" __declspec(dllexport) void DeleteCliPtr(CNETCLI* t);

class NETCLI : public CNETCLI
{
public:
	int _offset;

public:
	NETCLI(void);
	~NETCLI(void);
public:
	/** @brief ������㴫�����ݵ������ĺ��� */
	BOOL IP2Trans(WPARAM wparam, LPARAM lparam);
	/** @brief ������㴫�����ݵ���·��ĺ��� */
	BOOL IP2Link(WPARAM wparam, LPARAM lparam);
	/** @brief ����IP���ݱ����ײ������ */
	unsigned short iphdrmakesum(NETip_message *ip);
};