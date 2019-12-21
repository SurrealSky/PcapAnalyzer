// StreamsView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "StreamsView.h"
#include"PcapWindowDoc.h"


// CStreamsView

IMPLEMENT_DYNCREATE(CStreamsView, CListView)

CStreamsView::CStreamsView()
{

}

CStreamsView::~CStreamsView()
{
}

BEGIN_MESSAGE_MAP(CStreamsView, CListView)
	ON_MESSAGE(WM_STREAMVIEW_ADDSTREAM, &CStreamsView::OnStreamviewAddstream)
	ON_MESSAGE(WM_STREAMVIEW_ADDPACKET, &CStreamsView::OnStreamviewAddpacket)
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
	CString m_ColumnLabelStr[] = { "���","Time","Source","Destination","Protocol","sum" };

	//��ͷ�ֶ�

	CListCtrl& listctrl = GetListCtrl();//��ȡ�б�Ŀؼ�

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl��

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//�б���

	int width[6] = { 40,40,80, 80, 80, 40 };

	for (int i = 0; i < 6; i++)
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

void CStreamsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: �ڴ����ר�ô����/����û���
	//CPcapWindowDoc *pDoc=static_cast<CPcapWindowDoc*>(this->GetDocument());
	return;
}


afx_msg LRESULT CStreamsView::OnStreamviewAddstream(WPARAM wParam, LPARAM lParam)
{
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CSyncStream *stream = static_cast<CSyncStream*>((void*)wParam);
	if (stream)
	{
		CListCtrl &p=GetListCtrl();
		unsigned int count = p.GetItemCount();
		//�����
		CString item = "";
		item.Format("%d", count + 1);
		p.InsertItem(count, item);

		////ȡ��һ������ʱ���
		//p.SetItemText(count, 1, p->CACap.FormatTime(stream->GetTime()).c_str());

		//CNetInfo net = stream->GetNetInfo();
		//char ip[0x30] = { 0 };
		//inet_ntop(AF_INET, (void*)&(net.srcip), ip, 16);
		//item.Format("%s", ip);
		//p.SetItemText(count, 2, item);

		//inet_ntop(AF_INET, (void*)&(net.dstip), ip, 16);
		//item.Format("%s", ip);
		//p.SetItemText(count, 3, item);

		//p.SetItemText(count, 4, p->CACap.Protocol2String(net.proto).c_str());

		item.Format("%d", stream->GetCount());
		p.SetItemText(count, 5, item);

		p.SetItemData(count, (DWORD_PTR)(stream));
	}
	return 0;
}


afx_msg LRESULT CStreamsView::OnStreamviewAddpacket(WPARAM wParam, LPARAM lParam)
{
	//������ݰ�
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CSyncStream *stream = static_cast<CSyncStream*>((void*)wParam);
	if (stream)
	{
		CListCtrl &p = GetListCtrl();
		unsigned int count = p.GetItemCount();
		for (int i = 0; i < count ; i++)
		{
			if (((CSyncStream*)(p.GetItemData(i)))->guid == stream->guid)
			{
				CString item = "";
				item.Format("%d", stream->GetCount());
				p.SetItemText(i, 5, item);
				//�жϵ�ǰ���Ƿ�Ϊѡ����
				POSITION Position = p.GetFirstSelectedItemPosition();
				int Item = p.GetNextSelectedItem(Position);
				if (Item != -1)
				{
					if (((CSyncStream*)(p.GetItemData(Item)))->guid == stream->guid)
					{
						//if (p->mPackets.GetItemCount() > 0)
						//{
						//	//����
						//	CString strExp;
						//	p->dfilterEdt.GetWindowTextA(strExp);
						//	p->AddPacket2UI(stream->GetBack(), strExp.GetBuffer(0));
						//}
					}
				}
				break;
			}
		}
	}
	
	return 0;
}
