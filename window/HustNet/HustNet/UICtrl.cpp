/**@file
*@brief  �������API
*@date 2015.4.20
*@version <0.1>
*@note
*������Ƶ�ʵ��
*/
#pragma once
#include  "stdafx.h"
#include  "HustNet.h"
#include  "UICtrl.h"
#include  "MainFrm.h"
#include  "CmdView.h"

CMainFrame *pframe;
/**
*��ӡһ�����ֵ���־����\n
*��������PRINT��Ϣ��LogView���ڣ������ַ�����װ��lparam�У�LogView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����\n
* @param log ��Ҫ��ӡ����־�ַ���
*/
void PrintLog(CString log)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.para1 = &log;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, LOGVIEW, (LPARAM)(&mycmd));
}


/**
*��ӡһ�����ֵ���Ӧ����\n
*��������PRINT��Ϣ��RespondView���ڣ������ַ�����װ��lparam�У�RespondView���ڽ�\n
*�յ�����Ϣ���ӡ��Ӧ�ַ�����RespondView����\n
* @param rp ��Ҫ��ӡ���ַ���
*/
void PrintRp(CString rp)
{
	CMD mycmd;
	mycmd.ID = PRINT;
	mycmd.para1 = &rp;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, (LPARAM)(&mycmd));
}

/**
*�����־����\n
*��������CLEAN��Ϣ��LogView���ڣ�LogView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����
*/
void CleanLog(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, LOGVIEW, LPARAM(&mycmd));
}

/**
*�����Ӧ����
*��������CLEAN��Ϣ��LogView���ڣ�RespondView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����
*/
void CleanRp(CString e)
{
	CMD mycmd;
	mycmd.ID = CLEAN;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}


/**
*������Ӧ���ڵ�����\n
*��������SETTEXT��Ϣ��RespondView���ڣ������ַ�����װ��lparam�У�RespondView���ڽ�\n
*�յ�����Ϣ���ӡ��Ӧ�ַ�����RespondView����\n
* @param e ��Ҫ���õ��ַ���
*/
void SetRp(CString e)
{
	CMD mycmd;
	mycmd.ID = SETTEXT;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, RPVIEW, LPARAM(&mycmd));
}

/**
*����CmdView��������ʾ��\n
*��������SETHINT��Ϣ��CmdView���ڣ������ַ�����װ��lparam�У�CmdView���ڽ�\n
*�յ�����Ϣ������HINT������ʾ��\n
* @param e ��Ҫ���õ�������ʾ��
*/
void SetHint(CString e)
{
	CMD mycmd;
	mycmd.ID = SETHINT;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}


/**
*����CmdView��������ʾ��ӹ�CmdView������\n
*��������TAKEOVERCMD��Ϣ��CmdView���ڣ������ַ�����װ��lparam�У�CmdView���ڽ�\n
*�յ�����Ϣ������HINT������ʾ�����CmdView��״̬�л����û�����ģʽ���Ӷ��û��������\n
*��CmdView�л�ȡ����\n
*param e ��Ҫ���õ�������ʾ��
*/
void TakeOverCmd(CString e)
{
	CMD mycmd;
	mycmd.ID = TAKEOVERCMD;
	mycmd.para1 = &e;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

/**
*��CmdView�л�ȡһ������\n
*�������û�����ģʽ�´�Cmd��ȡһ�����롣���������жϵ�ǰ�Ƿ����û�����ģʽ\n
* @retval null �����û�����ģʽ
*��������ѭ�����Է�ʽ����CmdView��ȡ�µ�һ�����룬��ȡ���в�����*/
CString  GetLine()
{
	static CString  myline;
	CMDMODE cmdflag;
	LINESTATE lineflag;
	CMD mycmd;
	mycmd.ID = GETCMDSTATE;
	mycmd.para1 = &cmdflag;
	mycmd.para2 = NULL;
	if (pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd)) &&cmdflag!=USER){
		PrintLog(_T("δ�ӹ�CmdView����"));
		return NULL;
	}
	lineflag = OLDLINE;
	mycmd.ID = GETLINESTATE;
	mycmd.para1 = &lineflag;
	while (!pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd)) && lineflag == OLDLINE)
		;
	ASSERT(lineflag == NEWLINE);
	mycmd.ID = GETCOMMAND;
	mycmd.para1 = &myline;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
	EndInput();
	return myline;
}

/**
*֪ͨ�����������ѻ�ȡ���\n
*��������ENDINPUT��Ϣ��CmdView��֪ͨ�û�ģʽ�³ɹ���ȡ��һ�����롣CmdView���յ��������\n
*���������������ʾ��ȴ���һ�������������*/
void EndInput()
{
	CMD mycmd;
	mycmd.ID = ENDINPUT;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}

void SendOut(CString e)
{
	pframe->SendMessage(SENDTONPC, (WPARAM)&e);
}

