// PacketsView.cpp : 实现文件
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "PacketsView.h"


// CPacketsView

IMPLEMENT_DYNCREATE(CPacketsView, CListView)

CPacketsView::CPacketsView()
{

}

CPacketsView::~CPacketsView()
{
}

BEGIN_MESSAGE_MAP(CPacketsView, CListView)
END_MESSAGE_MAP()


// CPacketsView 诊断

#ifdef _DEBUG
void CPacketsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CPacketsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPacketsView 消息处理程序
