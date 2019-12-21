// StreamsView.cpp : 实现文件
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
	CString m_ColumnLabelStr[] = { "序号","Time","Source","Destination","Protocol","sum" };

	//表头字段

	CListCtrl& listctrl = GetListCtrl();//获取列表的控件

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// 选中某行使整行高亮（只适用与report 风格的listctrl）

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//列表风格

	int width[6] = { 40,40,80, 80, 80, 40 };

	for (int i = 0; i < 6; i++)
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

void CStreamsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
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
		//添加流
		CString item = "";
		item.Format("%d", count + 1);
		p.InsertItem(count, item);

		////取第一个包的时间戳
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
	//添加数据包
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
				//判断当前项是否为选中项
				POSITION Position = p.GetFirstSelectedItemPosition();
				int Item = p.GetNextSelectedItem(Position);
				if (Item != -1)
				{
					if (((CSyncStream*)(p.GetItemData(Item)))->guid == stream->guid)
					{
						//if (p->mPackets.GetItemCount() > 0)
						//{
						//	//增加
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
