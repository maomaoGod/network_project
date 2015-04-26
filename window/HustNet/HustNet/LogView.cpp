/**
*LogView.cpp : ʵ���ļ�
*/
#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"

// LogView
IMPLEMENT_DYNCREATE(LogView, CEditView)


/**
*LogView���캯��\n
*���캯����������󣬲����м򵥵ĳ�ʼ��������������ʼ�������¼�ַ���Ϊ�գ�������¼�����Ϊ0��\n
*��ʾ��¼��0��ʼ����
*/
LogView::LogView()
{
	Log.Empty();  
	logcount = 0;
}

LogView::~LogView()
{
}

/**
*ʵ����Ϣ�ʹ������ӳ���ϵ
*/
BEGIN_MESSAGE_MAP(LogView, CEditView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_MESSAGE(PRINT, Print)
	ON_MESSAGE(CLEAN, Clean)
END_MESSAGE_MAP()


/**
*LogView ��ͼ
*/

void LogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	/**
	*TODO:  �ڴ���ӻ��ƴ���
	*/
}


/**
*LogView ���
*/

#ifdef _DEBUG
void LogView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void LogView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


/**
*LogView ��ʼ��\n
*��������ͼ���ӵ��ĵ��󴥷�����Ҫ��ɽ���ĳ�ʼ������\n
*����ʵ������������ã�����ͼ����������Ϊ��Times New Roman��������������������ڲ���
*/
void LogView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	/**
	*TODO:  �ڴ����ר�ô����/����û���
	*/
	CClientDC dc(this);
	static CFont  myfont;
	myfont.CreatePointFont(120, (LPCTSTR)_T("Times New Roman"));
	((CEdit *)this)->SetFont(&myfont);
}

/**
*param cs  ���崴�����ṹ��\n
*CmdView�������������\n
*�������ڼ�����������ǰ������������Ҫͨ�����ݽ����Ľṹ���ñ�����������Ӧ�Ĵ�����,\n
*������ķ��Ϊӵ�д�ֱ����������ʵ���Զ����У��Զ��ֽⳤ�ַ���Ϊ���У����Ϊ������״̬
*/
BOOL LogView::PreCreateWindow(CREATESTRUCT& cs)
{
	/**
	*TODO:  �ڴ����ר�ô����/����û���
	*/
	m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}

/**
*������굥��\n
*����������굥��ʱ����������ͨ������ѡ���ı������޸���Ӧ���ı������������θ���Ϣ����ֹ�޸���־��ʷ
*/
void LogView::OnLButtonDown(UINT nFlags, CPoint point)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ\n
	*return;
	*/
	CEditView::OnLButtonDown(nFlags, point);
}

/**
*�������˫��\n
*�����������˫��ʱ����������ͨ��˫��ѡ���ı������޸���Ӧ���ı������������θ���Ϣ����ֹ�޸���־��ʷ
*/
void LogView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}

/**
*���ΰ�������\n
*�������ڷ�ϵͳ��������ʱ������LogView����ʾ��־��ʷ��ͨ�����ΰ�����Ϣʹ�ò������û�ֱ������ı���־��ʷ
*/
void LogView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

/**
*@param  wparam  ��Ҫ��ӡ���ַ���  \n
*��LogView��ӡһ���������\n
*�����ڴ��ڽ��յ�PRINT��Ϣʱ����������Ҫ��ӡ���ַ�������Ϊwparam�������Զ���Ӽ�¼���
*�Զ���ӻ��з����������뽹������Ϊβ����ʵ�ֹ�����������������Ӷ������ƶ�
*/
LRESULT LogView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	CString temp;
	/**
	*@brief �����ʾ��ͼ�¼��ţ���ӻ��з���β��
	*/
	temp.Format(_T("log%d: %s \r\n"), logcount++, mystr);
	/**
	*@brief ���������ַ����ӵ���־��¼β��
	*/
	Log += temp;
	/**
	*@brief ���´�����ʾ
	*/
	SetWindowText(Log);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}
/**
*�����־����\n
*������LogView���յ�CLEAN��Ϣʱ�����������������־��¼����������־��ţ�\n
*ͬʱ���ý��㣬��ʹ�ù������ص���ʼλ�ã���֤���������ı���һ��
*/
LRESULT LogView::Clean(WPARAM wparam, LPARAM lparam)
{
	/**
	*@brief �����־
	*/
	Log.Empty();
	/**
	*@brief ���´�������
	*/
	SetWindowText(Log);
	logcount = 0;
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}