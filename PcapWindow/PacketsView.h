#pragma once


// CPacketsView 视图

class CPacketsView : public CListView
{
	DECLARE_DYNCREATE(CPacketsView)

protected:
	CPacketsView();           // 动态创建所使用的受保护的构造函数
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


