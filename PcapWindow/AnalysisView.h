#pragma once


// CAnalysisView 视图

class CAnalysisView : public CListView
{
	DECLARE_DYNCREATE(CAnalysisView)

protected:
	CAnalysisView();           // 动态创建所使用的受保护的构造函数
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


