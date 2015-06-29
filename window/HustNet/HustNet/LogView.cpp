/**
*LogView.cpp : ʵ���ļ�
*/
#include "stdafx.h"
#include "HustNet.h"
#include "LogView.h"

// LogView
IMPLEMENT_DYNCREATE(LogView, CEditView)


/**
* @author ACM2012
* @note    LogView���캯��
*/
LogView::LogView()
{
	Log.Empty();  
	logcount = 0;
}

/**
* @author ACM2012
* @note    LogView��������
*/
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
* @author ACM2012
* @param [in] pDC�豸���
* @note  ��ȡ�ĵ���ָ��
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
* @author ACM2012
* @note  LogView ��ʼ����������ͼ���ӵ��ĵ��󴥷�����Ҫ��ɽ���ĳ�ʼ������
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
* @author ACM2012
* @param cs  ���崴�����ṹ��
* @note  �������ڼ�����������ǰ������������Ҫͨ�����ݽ����Ľṹ���ñ�����������Ӧ�Ĵ�����,
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
* @author ACM2012
* @param point �����������
* @note  ������굥������������굥��ʱ����������ͨ������ѡ���ı������޸���Ӧ���ı���
*���������θ���Ϣ����ֹ�޸���־��ʷ
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
*@author ACM2012
*@param point �����������
*@note �����������˫��ʱ�������������˫��������ͨ��˫��ѡ���ı������޸���Ӧ���ı���
*���������θ���Ϣ����ֹ�޸���־��ʷ
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
*@author ACM2012
*@note ���ΰ������뱾�����ڷ�ϵͳ��������ʱ������LogView����ʾ��־��ʷ��ͨ��
*���ΰ�����Ϣʹ�ò������û�ֱ������ı���־��ʷ
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
*@author ACM2012
*@param  wparam  ��Ҫ��ӡ���ַ���  \n
*@note ��LogView��ӡһ��������������ڴ��ڽ��յ�PRINT��Ϣʱ����������Ҫ��ӡ���ַ�������
*Ϊwparam�������Զ���Ӽ�¼����Զ���ӻ��з����������뽹������Ϊβ����ʵ�ֹ�������������
*�����Ӷ������ƶ�
*/
LRESULT LogView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	CString temp;
	temp.Format(_T("log%d: %s \r\n"), logcount++, mystr);
	Log += temp;
	SetWindowText(Log);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}

/**
*@author ACM2012
*@param  wparam  ��Ҫ��ӡ���ַ���  \n
*@note*�����־���ں�����LogView���յ�CLEAN��Ϣʱ�����������������־��¼��
*��������־��ţ�ͬʱ���ý��㣬��ʹ�ù������ص���ʼλ�ã���֤���������ı���һ��
*/
LRESULT LogView::Clean(WPARAM wparam, LPARAM lparam)
{
	Log.Empty();
	SetWindowText(Log);
	logcount = 0;
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}