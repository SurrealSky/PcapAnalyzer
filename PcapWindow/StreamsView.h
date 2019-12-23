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
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
protected:
	afx_msg LRESULT OnStreamviewAddstream(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnStreamviewAddpacket(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
};


