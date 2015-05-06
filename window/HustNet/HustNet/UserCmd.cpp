#include "stdafx.h"
#include "UserCmd.h"
#include "Tools.h"
#include "UICtrl.h"
#include "NetHtml.h"
#include "NetWork.h"
#include "Cmysocket.h"

using namespace Tools;
using namespace NetWork;

UserCmd::UserCmd()
{
}


UserCmd::~UserCmd()
{
}

/**
*ͨ����̬���ӿ�ʵ�ּӷ�����\n
*����ͨ����̬���ӿ�ʵ�ּӷ����㡣�������ȶ�������ʽ���н�������ȡ����������\n
*Ȼ��Ӷ�̬���ӿ��м��ؼӷ������������øú������м��㣬����ʽ�ͼ�������ӡ��\n
*RespondView����
* @param e  ���㹫ʽ�ַ���
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

/**
*ʵ�ֺͷ�������TCPͨ��\n
*����ʵ�ֺͷ�������ͨ�ţ��ͻ��˷������ݵ�����������������Ӧ��������Ӧ���ġ�\n
*�������Ƚӹ�cmd,ͨ������CSocket�ͷ������������ӣ��󶨶˿ڹ̶�Ϊ6500��\n
*��ȡһ�����룬���͸��ַ������������������մӷ��񷵻ص���Ϣ������ӡ��RespondView\n
*���ڣ�����ȡ������Ϊexit����������ѭ�������ر�����*/
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

void UserCmd::Http(CString e)
{
	//HttpSend Http;
	//Http.Begin();
	AppLayerHttp html;
	html.Begin();
}

void UserCmd::Chat(CString e){
	ChatWork chat;
	chat.Begin();
}

/**
* ����ͨ��·������Dll\n
* ����ͨ����̬���ӿ�·�����Լ���ָ��λ�õĶ�̬���ӿ⣬������ʧ�ܣ���ʧ����Ϣ��ӡ��״̬�������
* @param ��̬���ӿ�·��
*/
void UserCmd::SetDll(CString e){
	if (mydll.LoadDll(e) == FALSE){
	    PrintLog(_T("δ�ҵ�ָ���Ķ�̬���ӿ�"));
		return;
	}
}

/**
*����ͨ��url��ȡ��ҳhtml���ַ���\n
*����ͨ��url��ȡ��ҳhtml���ַ�������������ʾ����Ӧ�������
* @param url  ������ҳhtml��ͳһ��Դ�����
*/
void UserCmd::GetHtml(CString url){
	NetHtml myhtml;
	CString html = myhtml.getURLContext(url);
	SetRp(html);
}

/**
*�����Ӧ����
*/
void UserCmd::cleanrp(CString e){
	CleanRp(NULL);
}

/**
*���״̬����
*/
void UserCmd::cleanlog(CString e){
    CleanLog(NULL);
}

void UserCmd::Visit(CString e){
	if (e.Compare(_T("http")) == 0){
		AppLayerHttp myhttp;
		myhttp.Begin();
	}
}

void UserCmd::sendout(CString e){
	SendOut(e);
}

void UserCmd::mytest(CString e){
	CString sed;
	TakeOverCmd(_T("Test>"));
	Cmysocket mysock;
	mysock.Connect(_T("127.0.0.1"),6500);
	while ((sed = GetLine()).Compare(_T("exit")) != 0){
		mysock.Send(sed,sed.GetLength()*sizeof(TCHAR));
	}
}