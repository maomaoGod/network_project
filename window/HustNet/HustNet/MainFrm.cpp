/**
*MainFrm.cpp : CMainFrame ���ʵ��
*/

#include "stdafx.h"
#include "HustNet.h"
#include "MainFrm.h"
#include "NetSet.h"
#include "UIctrl.h"
#include <sys/stat.h>
#include <sys/types.h>   
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/**
*CMainFrame
*/
extern  CMainFrame *pframe;
IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(DISPATCH,Dispatch)
	ON_WM_CLOSE()
	ON_COMMAND(ID_NETSET, &CMainFrame::OnNETSET)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	/**
	*״̬��ָʾ��
	*/
	ID_SEPARATOR,           
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/**
*CMainFrame ����/����
*/

CMainFrame::CMainFrame()
{
	/**
	*TODO:  �ڴ���ӳ�Ա��ʼ������
	*/
	if (_access("cache", 0) == 0){
		if (_access("cache/html",0)==0)
			;
		else 
			CreateDirectory(_T("cache/html"), NULL);
	}
	else{
		CreateDirectory(_T("cache"), NULL);
		CreateDirectory(_T("cache/html"), NULL);
	}
	flag = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("δ�ܴ���������\n");
		/**
		*δ�ܴ���
		*/
		return -1;      
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	// TODO:  �������Ҫ��ͣ������������ɾ��������
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	//set the state bar Msg

	return 0;
}
/**
*@brief ���������������
*@param cs  �������Խṹ��
*@note �����ڴ��弴������ʱ��������ͨ���������Խṹ������ã��������ÿ��
*�ĸ߶ȺͿ�ȷֱ�Ϊ��Ļ�ֱ��ʵ�3/4��5/8������֤���ڵ���ʾλ�����þ���
*/
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style &= ~FWS_ADDTOTITLE;
	/**
	*cs.cx = 900;
	*cs.cy = 600;
	*���øߡ��� (��Ļ�ֱ��ʵ�3/4)*/
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN) * 3 / 4;
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN) * 5 / 8;
	/**
	*���ô���λ�� (����)
	*/
	cs.y = (cs.cy*4/3 - cs.cy) / 2;
	cs.x = (cs.cx*8/5 - cs.cx) / 2;
	return TRUE;
}

/**
*CMainFrame ���
*/

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


/**
*@author ACM2012
*@noteʵ�ִ��ڷָ�\n
*�����ڿ���ഴ���ͻ���ʱ������������ͨ��CSplitterWnd�Կͻ������зָ����ʹ��\n
*CSplitterWnd����ܿͻ�����Ϊ�����֣��ָ�Ľ����1���ͻ�����Ϊ1*2�����������Ҳ�\n
*����Ϊ��Ӧ����RespondView��������Ȼ����ʹ��CSplitterWnd�����(0,0)���񻮷�Ϊ2*1��\n
*������,�ϲര��ΪCmdView���������²ര��ΪLogView���������ָ���ɺ󽫶�Ӧ��View\n
*��̬������ָ������
*/

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	/**
	*TODO:  �ڴ����ר�ô����/����û���
	*/
	CRect rect;
	GetClientRect(&rect);
	BOOL bCreateSplit = m_splitter.CreateStatic(this, 1, 2);
	if (!s_splitter.CreateStatic(&m_splitter, 2, 1, WS_CHILD | WS_VISIBLE, m_splitter.IdFromRowCol(0, 0)))
	{
		return FALSE;
	}
	s_splitter.CreateView(0, 0, RUNTIME_CLASS(CmdView), CSize(rect.Width() / 2, rect.Height() / 2), pContext);
	s_splitter.CreateView(1, 0, RUNTIME_CLASS(LogView), CSize(rect.Width() / 2, rect.Height() / 4), pContext);
	m_splitter.CreateView(0, 1, RUNTIME_CLASS(RespondView), CSize(rect.Width() / 2, rect.Height()), pContext);
	CWnd *pwnd;
	pframe = this;
	pwnd = this->s_splitter.GetPane(0, 0);
	pcmd = DYNAMIC_DOWNCAST(CmdView, pwnd);
	pwnd = this->s_splitter.GetPane(1, 0);
	plog = DYNAMIC_DOWNCAST(LogView, pwnd);
	pwnd = this->m_splitter.GetPane(0, 1);
	prespond = DYNAMIC_DOWNCAST(RespondView, pwnd);
	flag = true;
	return bCreateSplit;
}

/**
*@author ACM2012
*@note ����仯ʱ��̬���������С�����ڿ����С�����仯ʱ���������������ݴ���ı仯
*���µ����Ӵ���Ĵ�С�������Ĺ���Ϊ��ര��Ŀ��Ϊ�����ͻ�����С��1/3���Ҳ�Ϊ2/3��
*�������С����ĸ߶Ⱦ�Ϊ�ͻ����߶ȵ�1/2�����д���ĸߺͿ������С��50���أ�����ϵͳ
*���ش���
*/

void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);
	/**
	*TODO:  �ڴ˴������Ϣ����������
	*/
	if (flag){
		if (nType != SIZE_MINIMIZED){
			CRect rect;
			/**
			*@brief ��ȡ�ͻ�����С
			*/
			GetClientRect(&rect);
			/**
			*@brief ���ô����С
			*/
			m_splitter.SetRowInfo(0, cy, 50);
			m_splitter.SetColumnInfo(0, rect.Width() / 3, 50);
			s_splitter.SetColumnInfo(0, rect.Width() / 3, 50);
			/**
			*@brief ���´���
			*/
			m_splitter.RecalcLayout();
		}
	}
}


/**
*@author ACM2012
*@param  wparam ������Ϣ��Ҫ���͵�Ŀ�Ĵ��ڱ��   lparam  ��Ϣ���͸�Ŀ�Ĵ��ڵĲ���
*@note �����ڽ��յ�DISPATCH��Ϣʱ��������������������wparamȷ����ϢҪ�����͵�Ŀ�Ĵ��ڣ�
*������lparam�е����ݣ�lparamָ��һ��CMD�ṹ�壬�ýṹ���һ��Ԫ��ΪҪ���͸�Ŀ�Ĵ�
*�ڵ���ϢID���ڶ�������ΪҪ���͸�Ŀ�Ĵ��ڵĲ���args
*/
LRESULT CMainFrame::Dispatch(WPARAM wparam, LPARAM lparam)
{
	int CMD_ID;
	void *CMD_PARA1, *CMD_PARA2;
	CMD_ID = ((CMD *)lparam)->ID;
	CMD_PARA1 = ((CMD *)lparam)->para1;
	CMD_PARA2 = ((CMD *)lparam)->para2;
	switch (wparam)
	{
	case CMDVIEW: 
		pcmd->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	case RPVIEW:
		prespond->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	case LOGVIEW: 
		plog->SendMessage(CMD_ID, (WPARAM)CMD_PARA1, (LPARAM)CMD_PARA2);
		break;
	default: break;
	}
	return 0;
}

/**
*@author ACM2012
*@note ����رճ���,��̬Ч���ر�������
*/
void CMainFrame::OnClose()
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/
	CleanRp(NULL);
	if (AfxMessageBox(_T(" �Ƿ��˳���� \n"), MB_OKCANCEL, MB_ICONQUESTION) == IDOK){
		CRect rectDlg;
		this->GetWindowRect(rectDlg);
		while (rectDlg.TopLeft().y+3 < rectDlg.BottomRight().y-3){//rectDlg.Height() > 30
			rectDlg.TopLeft().y += 3;
			rectDlg.BottomRight().y -= 3;
			this->MoveWindow(rectDlg);
		}
		DestroyWindow();//CFrameWnd::OnCloseWithout(0);
	}
	return;
}

/**
*@author ACM2012
*@note ���������öԻ���
*/
void CMainFrame::OnNETSET()
{
	/**
	*TODO:  �ڴ���������������
	*/
	/**
	*@brief ʵ�������öԻ���
	*/
	NetSet SetDlg;
	/**
	*@brief ģ̬��ʾ���öԻ���
	*/
	SetDlg.DoModal();
}
