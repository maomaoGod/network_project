
/**@file
*date 2015.4.20
*@version <0.1>
*@brief HustNetDoc.h : CHustNetDoc ��Ľӿ�
*/


#pragma once


class CHustNetDoc : public CDocument
{
protected: 
	/** 
	*�������л�����
	*/
	CHustNetDoc();
	DECLARE_DYNCREATE(CHustNetDoc)

/**
*����
*/
public:

/**
*����
*/
public:

/**
*��д
*/
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

/**
*ʵ��
*/
public:
	virtual ~CHustNetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

/**
*���ɵ���Ϣӳ�亯��
*/
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// ����Ϊ����������������������ݵ� Helper ����
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
