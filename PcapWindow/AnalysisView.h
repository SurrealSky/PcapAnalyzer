#pragma once


// CAnalysisView ��ͼ

class CAnalysisView : public CListView
{
	DECLARE_DYNCREATE(CAnalysisView)

protected:
	CAnalysisView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CAnalysisView();

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
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	afx_msg LRESULT OnStreamviewAddpacket(WPARAM wParam, LPARAM lParam);
};

