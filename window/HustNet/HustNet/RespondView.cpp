/**
*RespondView.cpp : ʵ���ļ�
*/
#include "stdafx.h"
#include "HustNet.h"
#include "RespondView.h"

IMPLEMENT_DYNCREATE(RespondView, CEditView)


/**
*RespondView���캯��
*���캯����������󣬲����м򵥵ĳ�ʼ����������ʼ����Ӧ��¼Ϊ0*/
RespondView::RespondView()
{
	Res.Empty();
}

RespondView::~RespondView()
{
}

BEGIN_MESSAGE_MAP(RespondView, CEditView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_MESSAGE(PRINT,Print)
	ON_MESSAGE(CLEAN,Clean)
	ON_MESSAGE(SETTEXT,SetText)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


/**
*RespondView ��ͼ
*/

void RespondView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	/**
	*TODO:  �ڴ���ӻ��ƴ���
	*/
	SetWindowText(Res);
}


/**
*RespondView ���
*/

#ifdef _DEBUG
void RespondView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void RespondView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


/**
*RespondView ��Ϣ�������\n
*RespondView ��ʼ��
*��������ͼ���ӵ��ĵ��󴥷�����Ҫ��ɽ���ĳ�ʼ������\n
*����ʵ������������ã�����ͼ����������Ϊ��Times New Roman��������������������ڲ���
*/
void RespondView::OnInitialUpdate()
{
	CEditView::OnInitialUpdate();

	/**
	*TODO:  �ڴ����ר�ô����/����û���
	*/
	CClientDC dc(this);
	dc.GetTextMetrics(&tm);
	static CFont  myfont;
	myfont.CreatePointFont(120, (LPCTSTR)_T("Times New Roman"));
	((CEdit *)this)->SetFont(&myfont);
}

/**
* @param cs  ���崴�����ṹ��
* RespondView�������������\n
* �������ڼ�����������ǰ������������Ҫͨ�����ݽ����Ľṹ���ñ�����������Ӧ�Ĵ�����\n
* ������ķ��Ϊӵ�д�ֱ����������ʵ���Զ����У��Զ��ֽⳤ�ַ���Ϊ���У����Ϊ������״̬
*/
BOOL RespondView::PreCreateWindow(CREATESTRUCT& cs)
{
	/**
	*TODO:  �ڴ����ר�ô����/����û���
	*/
		m_dwDefaultStyle = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL |ES_MULTILINE | ES_NOHIDESEL;
	return CCtrlView::PreCreateWindow(cs);
}

/**
*������굥��\n
*����������굥��ʱ����������ͨ������ѡ���ı������޸���Ӧ���ı������������θ���Ϣ����ֹ�޸���Ӧ��ʷ
*/
void RespondView::OnLButtonDown(UINT nFlags, CPoint point)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/

	//CEditView::OnLButtonDown(nFlags, point);
}

/**
*�������˫��\n
*�����������˫��ʱ����������ͨ��˫��ѡ���ı������޸���Ӧ���ı������������θ���Ϣ����ֹ�޸���Ӧ��ʷ
*/
void RespondView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/
	return;
	//CEditView::OnLButtonDblClk(nFlags, point);
}

/**
*���ΰ�������\n
*�������ڷ�ϵͳ��������ʱ������RespondView����ʾ��־��ʷ��ͨ�����ΰ�����Ϣʹ�ò������û�ֱ������ı���Ӧ��ʷ
*/
void RespondView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	/**
	*TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	*/
	return;
	//CEditView::OnChar(nChar, nRepCnt, nFlags);
}

/**
*��RespondView��ӡһ���������\n
*�����ڴ��ڽ��յ�PRINT��Ϣʱ����������Ҫ��ӡ���ַ�������Ϊwparam�������Զ���Ӽ�¼���\n
*���Զ���ӻ��з����������뽹������Ϊβ����ʵ�ֹ�����������������Ӷ������ƶ�
* @param  wparam  ��Ҫ��ӡ���ַ���  
*/

LRESULT RespondView::Print(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	Res += /*_T("Respond: ") +*/ mystr + _T("\r\n");
	SetWindowText(Res);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l,l,false);
	((CEdit *)this)->SetFocus();
	return 0;
}

/**
*�����־����\n
*������LogView���յ�CLEAN��Ϣʱ�����������������־��¼����������־��ţ�\n
*ͬʱ���ý��㣬��ʹ�ù������ص���ʼλ�ã���֤���������ı���һ��*/
LRESULT RespondView::Clean(WPARAM wparam, LPARAM lparam)
{
	Res.Empty();
	SetWindowText(Res);
	int l = ((CEdit *)this)->GetWindowTextLength();
	((CEdit *)this)->SetSel(l, l, false);
	((CEdit *)this)->SetFocus();
	return 0;
}

/**
*������Ӧ��������\n
*������RespondView���յ�SETTEXT��Ϣʱ�����������������־���ڣ�\n
*������������Ϊָ�����ַ������ݡ�
* @param  wparam  ���õĴ��������ַ���
*/
LRESULT RespondView::SetText(WPARAM wparam, LPARAM lparam)
{
	CString mystr = *((CString *)wparam);
	Res = mystr;
	SetWindowText(Res);
	return 0;
}

