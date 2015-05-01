#pragma once
#include "stdafx.h"
/**
*UICtrl��Ҫʵ���˴ӽ��������������ܣ�ΪUserCmd�����Զ�������ṩ��������ӿڡ��Զ������ͨ������UICtrl��ȫ�ֺ���
*ʵ���ڿ�ܽ�������������ͨ��UICtrl���Դ�ӡ���ֵ�LogView,RespondView���ڣ��������LogView��RespondView�������ݣ�
*�������û�����ģʽ�´�CmdView�л�ȡһ�����롣UICtrl�ṩ�ӹ�CmdView���ڵĹ��ܣ�Ҳ��������������ʾ�UICtrl����ֱ
*�Ӳ������ڣ�����ͨ����ָ�����ڷ�������ķ�ʽ֪ͨ��Ӧ������ɡ�
*/
typedef enum   { OLDLINE, NEWLINE }  LINESTATE;
typedef enum   { THREAD_EXIT, THREAD_WAIT, THREAD_RUN }  STATE;
typedef enum   { ROOT, USER } CMDMODE;

/**
*@brief ��ӡһ�����ֵ���־����
* @param log ��Ҫ��ӡ����־�ַ���
*/
void PrintLog(CString log);
/**
*@brief ��ӡһ�����ֵ���Ӧ����
*@param rp ��Ҫ��ӡ���ַ���
*/
void PrintRp(CString rp);
/**
*@brief �����Ӧ����
*��������CLEAN��Ϣ��LogView���ڣ�RespondView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����
*/
void CleanRp(CString e);
/**
*@brief �����־����\n
*/
void CleanLog(CString e);
/**
*@brief ������Ӧ���ڵ�����
*@param e ��Ҫ���õ��ַ���
*/
void SetRp(CString e);
/**
*Brief ����CmdView��������ʾ��
*@param e ��Ҫ���õ�������ʾ��
*/
void SetHint(CString e);
/**
*@brief ����CmdView��������ʾ��ӹ�CmdView������
*param e ��Ҫ���õ�������ʾ��
*/
void TakeOverCmd(CString e);
/**
*@brief ��CmdView�л�ȡһ������
*@retval null �����û�����ģʽ
*/
CString  GetLine();
/**
*@brief ֪ͨ�����������ѻ�ȡ���*/
void EndInput();

void SendOut(CString);
