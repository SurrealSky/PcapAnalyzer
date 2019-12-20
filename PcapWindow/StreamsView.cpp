// StreamsView.cpp : 实现文件
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "StreamsView.h"


// CStreamsView

IMPLEMENT_DYNCREATE(CStreamsView, CListView)

CStreamsView::CStreamsView()
{

}

CStreamsView::~CStreamsView()
{
}

BEGIN_MESSAGE_MAP(CStreamsView, CListView)
END_MESSAGE_MAP()


// CStreamsView 诊断

#ifdef _DEBUG
void CStreamsView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CStreamsView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStreamsView 消息处理程序
void CStreamsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CString m_ColumnLabelStr[] = { "学号", "姓名", "专业", "籍贯" };

	//表头字段

	CListCtrl& listctrl = GetListCtrl();//获取列表的控件

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// 选中某行使整行高亮（只适用与report 风格的listctrl）

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//列表风格

	int width[6] = { 80, 80, 110, 150 };

	for (int i = 0; i < 4; i++)
	{
		listctrl.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // 设置表头
	}
}


BOOL CStreamsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style = cs.style | LVS_REPORT;// 设置成报告列表的显示形式
	return CListView::PreCreateWindow(cs);
}