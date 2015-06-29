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
*@author ACM2012
*@param e ����CmdView��������ʾ��
*@note *��������PRINT��Ϣ��LogView���ڣ������ַ�����װ��lparam�У�
*LogView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����\n
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
*@author ACM2012
*@param rp ��Ҫ��ӡ���ַ���
*@note��ӡһ�����ֵ���Ӧ����\n
*��������PRINT��Ϣ��RespondView���ڣ������ַ�����װ��lparam�У�
*RespondView���ڽ��յ�����Ϣ���ӡ��Ӧ�ַ�����RespondView����
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
*@author ACM2012
*@param rp ��Ҫ��ӡ���ַ���
*@note�����־���ں�������CLEAN��Ϣ��LogView���ڣ�LogView���ڽ��յ�
*����Ϣ���ӡ��Ӧ�ַ�����LogView����
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
*@author ACM2012
*@note�����Ӧ���ں�������CLEAN��Ϣ��LogView���ڣ�RespondView����
*���յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����
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
*@author ACM2012
*@param ������Ӧ���ڵ������ַ���
*@note�����Ӧ���ں�������CLEAN��Ϣ��LogView���ڣ�RespondView����
*���յ�����Ϣ���ӡ��Ӧ�ַ�����LogView����
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
*@author ACM2012
*@param e ����CmdView��������ʾ��
*@note��������SETHINT��Ϣ��CmdView���ڣ������ַ�����װ��lparam�У�CmdView���ڽ�
*�յ�����Ϣ������HINT������ʾ��
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
*@author ACM2012
*@param e ��Ҫ���õ�������ʾ��
*@note��������TAKEOVERCMD��Ϣ��CmdView���ڣ������ַ�����װ��lparam�У�CmdView���ڽ�
*�յ�����Ϣ������HINT������ʾ�����CmdView��״̬�л����û�����ģʽ���Ӷ��û��������
*��CmdView�л�ȡ����
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
*@author ACM2012
*@param e ��Ҫ���õ�������ʾ��
*@note��CmdView�л�ȡһ�����뺯�����û�����ģʽ�´�Cmd��ȡһ�����롣
*���������жϵ�ǰ�Ƿ����û�����ģʽ,��������ѭ�����Է�ʽ����CmdView��
*ȡ�µ�һ�����룬��ȡ���в�����
*/
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
*@author ACM2012
*@param e ��Ҫ���õ�������ʾ��
*@note ֪ͨ�����������ѻ�ȡ��ɺ�������ENDINPUT��Ϣ��CmdView��֪ͨ�û�ģʽ�³ɹ���ȡ��
*һ�����롣CmdView���յ��������,���������������ʾ��ȴ���һ�������������
*/
void EndInput()
{
	CMD mycmd;
	mycmd.ID = ENDINPUT;
	mycmd.para1 = NULL;
	mycmd.para2 = NULL;
	pframe->SendMessage(DISPATCH, CMDVIEW, LPARAM(&mycmd));
}
