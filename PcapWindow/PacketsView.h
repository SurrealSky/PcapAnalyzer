#pragma once


// CPacketsView ��ͼ

class CPacketsView : public CListView
{
	DECLARE_DYNCREATE(CPacketsView)

protected:
	CPacketsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPacketsView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
};


