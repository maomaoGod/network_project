// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once
#include "NetProtocol.h"

class CNAT
{
public:
	NAT_translation_table* p;
	unsigned int initial_IP;

public:	
	/** @brief CNAT��Ĺ��캯�� */
	CNAT();
	/** @brief CNAT����������� */
	~CNAT();
    /*<����ԴIP��Դ�˿ڡ�Ŀ��IP��Ŀ�Ķ˿ڵĵ�ַ����NatЭ���޸� */
	In_NAT* NAT(In_NAT* Nat);
};