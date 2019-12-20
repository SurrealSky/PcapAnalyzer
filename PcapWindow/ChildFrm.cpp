
// ChildFrm.cpp : CChildFrame ���ʵ��
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

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame ���

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

// CChildFrame ��Ϣ�������


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: �ڴ����ר�ô����/����û���
	//�������ڷָ�Ϊ1��2�У�
	m_wndSplitter.CreateStatic(this, 1, 2);
	//Ȼ���������ڵĵ�1�е�1���а�CMyListView
	m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	//m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyListView), CSize(100, 200), pContext);
	//���ָ������m_wndSplitter�ĵ�1�У�2���ٴν��зָ�ָ�Ϊ2��1��
	m_wndSplitter2.CreateStatic(&m_wndSplitter, 2, 1, WS_CHILD | WS_VISIBLE, m_wndSplitter.IdFromRowCol(0, 1));
	m_wndSplitter2.CreateView(0, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	m_wndSplitter2.CreateView(1, 0, RUNTIME_CLASS(CStreamsView), CSize(100, 200), pContext);
	return TRUE;
	//return CMDIChildWndEx::OnCreateClient(lpcs, pContext);
}
