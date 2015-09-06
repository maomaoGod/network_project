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
	/** @brief 从网络层传送数据到传输层的函数 */
	BOOL IP2Trans(WPARAM wparam, LPARAM lparam);
	/** @brief 从网络层传送数据到链路层的函数 */
	BOOL IP2Link(WPARAM wparam, LPARAM lparam);
	/** @brief 计算IP数据报的首部检验和 */
	unsigned short iphdrmakesum(NETip_message *ip);
};