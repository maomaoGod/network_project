
/**
*HustNet.cpp : ����Ӧ�ó��������Ϊ��
*/
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "HustNet.h"
#include "MainFrm.h"

#include "HustNetDoc.h"
#include "CmdView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


/**
*CHustNetApp
*/

BEGIN_MESSAGE_MAP(CHustNetApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CHustNetApp::OnAppAbout)
	/**
	*�����ļ��ı�׼�ĵ�����
	*/
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	/**
	*��׼��ӡ��������
	*/
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


/**
*CHustNetApp ����
*/

CHustNetApp::CHustNetApp()
{
	/**
	*֧����������������
	*/
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	/**
	*���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: \n
	* - �����д˸������ã�������������������֧�ֲ�������������\n
	* - ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	*/
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	/**
	*TODO:  ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����\n
    *������ַ�����ʽΪ CompanyName.ProductName.SubProduct.VersionInformation
	*/
	SetAppID(_T("HustNet.AppID.NoVersion"));

	/**
	*TODO:  �ڴ˴���ӹ�����룬\n
	*��������Ҫ�ĳ�ʼ�������� InitInstance ��
	*/
}

/**
*Ψһ��һ�� CHustNetApp ����
*/


CHustNetApp theApp;


/**
*CHustNetApp ��ʼ��
*/

BOOL CHustNetApp::InitInstance()
{
	/**
	*���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ\n
	*ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��\n
	*����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	*/
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	/**
	*��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�\n
	*�����ؼ��ࡣ
	*/
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	/**
	*��ʼ�� OLE ��
	*/ 

	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	/**
	*ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()\n	
	* AfxInitRichEdit2();

	*��׼��ʼ��\n
	*���δʹ����Щ���ܲ�ϣ����С\n
	* ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����\n
	*����Ҫ���ض���ʼ������\n
	*�������ڴ洢���õ�ע�����\n
	* TODO:  Ӧ�ʵ��޸ĸ��ַ�����\n
	* �����޸�Ϊ��˾����֯��
	*/
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	/**
	*���ر�׼ INI �ļ�ѡ��(���� MRU)
	*/
	LoadStdProfileSettings(4);  


	/**
	*ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��\n
	*�������ĵ�����ܴ��ں���ͼ֮�������
	*/
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CHustNetDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CmdView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	/**
	*������׼ shell ���DDE�����ļ�������������
	*/
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	/**
	*��������������ָ������� \n
	*����� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	*/
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	/**
	*Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	*/
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CHustNetApp::ExitInstance()
{
	/**
	*TODO:  �����������ӵĸ�����Դ
	*/
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

/**
*CHustNetApp ��Ϣ�������
*/


/**
*����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���
*/

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

/**
*�Ի�������
*/
	enum { IDD = IDD_ABOUTBOX };

protected:
	/** 
	*DDX/DDV ֧��
	*/
	virtual void DoDataExchange(CDataExchange* pDX);   

/**
*ʵ��
*/
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

/**
*�������жԻ����Ӧ�ó�������
*/
void CHustNetApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CHustNetApp ��Ϣ�������



