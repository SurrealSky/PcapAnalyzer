#pragma once


// CStreamsView ��ͼ

class CStreamsView : public CListView
{
	DECLARE_DYNCREATE(CStreamsView)

protected:
	CStreamsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CStreamsView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


