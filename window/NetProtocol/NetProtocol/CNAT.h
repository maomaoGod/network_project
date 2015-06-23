// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once
#include "NetProtocol.h"

class CNAT
{
public:
	NAT_translation_table* p;
	unsigned int initial_IP;

public:	
	/** @brief CNAT类的构造函数 */
	CNAT();
	/** @brief CNAT类的析构函数 */
	~CNAT();
    /*<输入源IP、源端口、目的IP、目的端口的地址，用Nat协议修改 */
	In_NAT* NAT(In_NAT* Nat);
};