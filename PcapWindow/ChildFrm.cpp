
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "ChildFrm.h"
#include"StreamsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO: 在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	//将主窗口分割为1行2列，
	m_wndSplitter.CreateStatic(this, 1, 2);
	//然后在主窗口的第1行第1列中绑定CMyListView
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	//m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyListView), CSize(100, 200), pContext);
	//将分割类对象m_wndSplitter的第1行，2列再次进行分割，分割为2行1列
	m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	return TRUE;
	//return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}
