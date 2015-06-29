#include "stdafx.h"
#include "UserCmd.h"
#include "Tools.h"
#include "UICtrl.h"
#include "Cmysocket.h"
#include "NetHtml.h"
#include "NetWork.h"
using namespace Tools;
using namespace NetWork;



UserCmd::UserCmd()
{
}


UserCmd::~UserCmd()
{
}

/**
* @author ACM2012
* @param e  ���㹫ʽ�ַ���
* @note  ͨ����̬���ӿ�ʵ�ּӷ����㺯��ͨ����̬���ӿ�ʵ�ּӷ����㡣�������ȶ���
*����ʽ���н�������ȡ����������Ȼ��Ӷ�̬���ӿ��м��ؼӷ������������øú�������
*���㣬����ʽ�ͼ�������ӡ��RespondView����
*/
void UserCmd::Compute(CString e){
	int pos;
	CString a, b, result;
	int c1, c2;
	pos = e.Find(_T('+'));
	a.Format(_T("%s"), e.Mid(0, pos));
	b.Format(_T("%s"), e.Mid(pos));
	c1 = _wtoi(a);
	c2 = _wtoi(b);
	typedef int(*ADDPROC)(int, int);
	ADDPROC Add = (ADDPROC)mydll.GetFun("add");
	if (Add == NULL){
		PrintLog(_T("δ���ض�̬���ӿ�"));
		return;
	}
	result.Format(_T("%d+%d=%d"), c1, c2, Add(c1, c2));
	PrintRp(result);
}

/*void UserCmd::Test(CString e){
	CString mystr;
	TakeOverCmd(_T("myTest>"));
	while ((mystr =GetLine()).Compare(_T("exit")) != 0)
		PrintRp(mystr);
}*/

/*
* @author ACM2012
* @note  ʵ�ֺͷ�������TCPͨ��\n
*����ʵ�ֺͷ�������ͨ�ţ��ͻ��˷������ݵ�����������������Ӧ��������Ӧ���ġ�\n
*�������Ƚӹ�cmd,ͨ������CSocket�ͷ������������ӣ��󶨶˿ڹ̶�Ϊ6500��\n
*��ȡһ�����룬���͸��ַ������������������մӷ��񷵻ص���Ϣ������ӡ��RespondView\n
*���ڣ�����ȡ������Ϊexit����������ѭ�������ر�����
*/
void  UserCmd::Connect(CString e){
	AfxSocketInit();
	/**
	*���� CSocket ����
	*/
	CSocket aSocket;

	CString strIP;
	strIP.Format(_T("%s"), _T("127.0.0.1"));
	if (!aSocket.Create())
	{
		CString error;
		error.Format(_T("����ʧ��:%d"), aSocket.GetLastError());
		PrintLog(error);
		return;
	}
	/**
	*ת����Ҫ���ӵĶ˿���������
	*/
	PrintLog(_T("�׽��ִ����ɹ�"));
	/**
	*����ָ���ĵ�ַ�Ͷ˿�
	*/
	if (aSocket.Connect(strIP, 6500))
	{
		PrintLog(_T("���ӷ������ɹ�"));
		TCHAR szRecValue[1024] = { 0 };
		CString  sed, rev;
		int times = 0;
		TakeOverCmd(_T("client>: "));
		aSocket.Receive((void *)szRecValue, 1024);
		rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
		PrintRp(rev);
		while ((sed = GetLine()).Compare(_T("exit")) != 0){
			aSocket.Send(sed, sed.GetLength()*sizeof(TCHAR));
			memset(szRecValue, 0, 1024 * sizeof(TCHAR));
			aSocket.Receive((void *)szRecValue, 1024);
			rev.Format(_T("���Է���������Ϣ:%s"), szRecValue);
			PrintRp(rev);
		}
	}
	else{
		CString error;
		error.Format(_T("���ӷ�����ʧ��:%d"), aSocket.GetLastError());
		PrintLog(error);
	}
	aSocket.Close();
}
/*
* @author ACM2012
* @note HttpЭ�����
*/
void UserCmd::Http(CString e)
{
	//HttpSend Http;
	//Http.Begin();
	AppLayerHttp html;
	html.Begin();
}
/*
* @author ACM2012
* @note ftpЭ�����
*/
void UserCmd::Ftp(CString e){
	FTPApp myftp;
	myftp.Begin();
}

void UserCmd::Chat(CString e){
	ChatWork chat;
	chat.Begin();
}

/*
* @author ACM2012
* @param ��̬���ӿ�·��
* @note * ����ͨ��·������Dll\n
* ����ͨ����̬���ӿ�·�����Լ���ָ��λ�õĶ�̬���ӿ⣬������ʧ�ܣ���ʧ����Ϣ��ӡ��״̬�������
*/
void UserCmd::SetDll(CString e){
	if (mydll.LoadDll(e) == FALSE){
	    PrintLog(_T("δ�ҵ�ָ���Ķ�̬���ӿ�"));
		return;
	}
}

/*
* @author ACM2012
* @param url  ������ҳhtml��ͳһ��Դ�����
* @note ����ͨ��url��ȡ��ҳhtml���ַ�������ͨ��url��ȡ��ҳhtml���ַ�������������ʾ����Ӧ�������
*/

#include "Httpworker.h"
void UserCmd::GetHtml(CString url){
	NetHtml myhtml;
	CString html = myhtml.getURLContext(url);
	system("pause");
	SetRp(html);
}

/*
* @author ACM2012
* @note �����Ӧ����
*/
void UserCmd::cleanrp(CString e){
	CleanRp(NULL);
}

/*
* @author ACM2012
* @note �����־����
*/
void UserCmd::cleanlog(CString e){
    CleanLog(NULL);
}


void UserCmd::mytest(CString e){
	CString sed;
	int count;
	TakeOverCmd(_T("Test>"));
	TCHAR S[100] = { 0 };
	CmySocket mysock;
	mysock.Connect(_T("172.21.127.1"), 6500);
	while ((sed = GetLine()).Compare(_T("exit")) != 0){
		mysock.Send(sed, sed.GetLength()*sizeof(TCHAR));
		count = 0;
		while (count < 1024) {
			count+=mysock.Receive(S, 100);
			PrintRp(S);
			memset(S, 0, 100 * sizeof(TCHAR));
		}
	}
	mysock.Close();
}