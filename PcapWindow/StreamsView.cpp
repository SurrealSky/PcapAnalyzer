// StreamsView.cpp : ʵ���ļ�
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


// CStreamsView ���

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


// CStreamsView ��Ϣ�������
void CStreamsView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CString m_ColumnLabelStr[] = { "ѧ��", "����", "רҵ", "����" };

	//��ͷ�ֶ�

	CListCtrl& listctrl = GetListCtrl();//��ȡ�б�Ŀؼ�

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl��

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//�б���

	int width[6] = { 80, 80, 110, 150 };

	for (int i = 0; i < 4; i++)
	{
		listctrl.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // ���ñ�ͷ
	}
}


BOOL CStreamsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style = cs.style | LVS_REPORT;// ���óɱ����б����ʾ��ʽ
	return CListView::PreCreateWindow(cs);
}