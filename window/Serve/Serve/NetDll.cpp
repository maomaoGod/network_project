/**@file
*@brief
*@author ������ ������
*@date 2015.4.20
*@version <0.1>
*@note
*����NetDll���ͷ�ļ�
*/
#include "stdafx.h"
#include "NetDll.h"

NetDll::NetDll()
{
	Hinst = NULL;
}

NetDll::~NetDll()
{
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
}

/**
 *ͨ��·�����ض�̬���ӿ�
 *����ʵ��ͨ��·�����ض�̬���ӿ⡣���������ͷ����о���������ڣ���Ȼ��ͨ··�����ض�̬���ӿ�
 * @param path  ��̬���ӿ���ַ���·��
 * @retval TRUE �ɹ�����
 * @retval FALSE ʧ��
 */
BOOL NetDll:: LoadDll(LPCTSTR path){
	/**
	*�ͷ����о��
	*/
	if (Hinst != NULL){   //�ͷ����о��
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	/**
	*��ȡ���ӿ�ʵ�����
	*/
	Hinst = LoadLibrary(path); //��ȡ���ӿ�ʵ�����
	if (Hinst == NULL){
		//PrintLog(_T("���ض�̬���ӿ�ʧ��"));
		return  FALSE;
	}
	//PrintLog(_T("���ض�̬���ӿ�ɹ�"));
	return TRUE;
}

/**
*����ͨ�����������Ա���Ķ�̬���ӿ������غ�����ַ
* @param name �����ú����ĺ�����
*/

FARPROC NetDll::GetFun(	LPCSTR name){
	if (Hinst == NULL)
		return NULL;
	return GetProcAddress(Hinst, name);
}

void NetDll::FreeDll(){
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	return;
}

/**
*ͨ���ļ��Ի�����ض�̬���ӿ�\n
*����ͨ�����ļ��Ի�����ض�̬���ӿ⡣��������һ���ļ��Ի��򴰿ڣ�\n
*��ȡѡ���ļ���·����Ȼ��ͨ������ȡ���ļ�·�����ض�̬���ӿ�
*/
void NetDll::OpenDll(){
	TCHAR  format[100] = _T("Files (*.dll)|*.dll||");
	/**
	*���ļ��Ի���
	*/
	CFileDialog file((int)TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, format);//���ļ��Ի���
	file.DoModal();
	CString path;
	/**
	*��ȡ���ӿ�·��
	*/
	path = file.GetPathName();//��ȡ���ӿ�·��
	if (path.IsEmpty())
		return;
	if (Hinst != NULL){
		FreeLibrary(Hinst);
		Hinst = NULL;
	}
	/**
	*��ȡ���ӿ�ʵ�����
	*/
	Hinst = LoadLibrary(path); //��ȡ���ӿ�ʵ�����
	if (Hinst == NULL){
		//PrintLog(_T("���ض�̬���ӿ�ʧ��!"));
		return;
	}
	//PrintLog(_T("���ض�̬���ӿ�ɹ�!"));
}

