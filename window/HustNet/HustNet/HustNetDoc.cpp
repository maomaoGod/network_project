
/**
*HustNetDoc.cpp : CHustNetDoc ���ʵ��
*/

#include "stdafx.h"
/**
*SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������\n
* ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
*/
#ifndef SHARED_HANDLERS
#include "HustNet.h"
#endif

#include "HustNetDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/** 
*CHustNetDoc
*/


IMPLEMENT_DYNCREATE(CHustNetDoc, CDocument)

BEGIN_MESSAGE_MAP(CHustNetDoc, CDocument)
END_MESSAGE_MAP()


/**
*CHustNetDoc ����/����
*/

CHustNetDoc::CHustNetDoc()
{
	/**
	*TODO:  �ڴ����һ���Թ������
	*/

}

CHustNetDoc::~CHustNetDoc()
{
}

BOOL CHustNetDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	/**
	*TODO:  �ڴ�������³�ʼ������\n
	*(SDI �ĵ������ø��ĵ�)
	*/

	return TRUE;
}




/**
*CHustNetDoc ���л�
*/

void CHustNetDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		/**
		*TODO:  �ڴ���Ӵ洢����
		*/
	}
	else
	{
		/**
		*TODO:  �ڴ���Ӽ��ش���
		*/
	}
}

#ifdef SHARED_HANDLERS

/**
*����ͼ��֧��
*/
void CHustNetDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	/**
	*�޸Ĵ˴����Ի����ĵ�����
	*/
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

/**
*������������֧��
*/
void CHustNetDoc::InitializeSearchContent()
{
	CString strSearchContent;
	/**
	*���ĵ����������������ݡ�\n
	* ���ݲ���Ӧ�ɡ�;���ָ�\n

	* ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��\n
	* SetSearchContent(strSearchContent);
	*/
}

void CHustNetDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}
/**
*SHARED_HANDLERS
*/
#endif 

/**
*CHustNetDoc ���
*/

#ifdef _DEBUG
void CHustNetDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHustNetDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
/**
*_DEBUG
*/
#endif 


/**
*CHustNetDoc ����
*/
