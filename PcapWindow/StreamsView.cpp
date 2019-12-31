// StreamsView.cpp : 实现文件
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "StreamsView.h"
#include"PcapWindowDoc.h"
#include <WS2tcpip.h>


// CStreamsView

IMPLEMENT_DYNCREATE(CStreamsView, CListView)

CStreamsView::CStreamsView()
{

}

CStreamsView::~CStreamsView()
{
}

BEGIN_MESSAGE_MAP(CStreamsView, CListView)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CStreamsView::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CStreamsView::OnNMClick)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CStreamsView 诊断

#ifdef _DEBUG
void CStreamsView::AssertValid() const
{
	//CListView::AssertValid();
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
	if (lHint == WM_STREAMVIEW_ADDSTREAM)
	{
		CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
		CSyncStream *stream = static_cast<CSyncStream*>((void*)pHint);
		if (stream)
		{
			CListCtrl &p = GetListCtrl();
			unsigned int count = p.GetItemCount();
			//添加流
			CString item = "";
			item.Format("%d", count + 1);
			p.InsertItem(count, item);

			p.SetItemText(count, 1, pDoc->CACap.FormatTime(stream->time).c_str());

			CNetInfo net = stream->net;
			char ip[0x30] = { 0 };
			inet_ntop(AF_INET, (void*)&(net.srcip), ip, 16);
			item.Format("%s", ip);
			p.SetItemText(count, 2, item);

			inet_ntop(AF_INET, (void*)&(net.dstip), ip, 16);
			item.Format("%s", ip);
			p.SetItemText(count, 3, item);

			p.SetItemText(count, 4, pDoc->CACap.Protocol2String(net.proto).c_str());

			item.Format("%d", stream->GetCount());
			p.SetItemText(count, 5, item);

			p.SetItemData(count, (DWORD_PTR)(stream));
		}
	}
	else if (lHint == WM_STREAMVIEW_ADDPACKET)
	{
		//添加数据包
		CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
		CSyncStream *stream = static_cast<CSyncStream*>((void*)pHint);
		if (stream)
		{
			CListCtrl &p = GetListCtrl();
			unsigned int count = p.GetItemCount();
			for (int i = 0; i < count; i++)
			{
				if (((CSyncStream*)(p.GetItemData(i)))->guid == stream->guid)
				{
					CString item = "";
					item.Format("%d", stream->GetCount());
					p.SetItemText(i, 5, item);
					break;
				}
			}
		}
	}
	else if (lHint == WM_VIEW_CLEAR)
	{
		CListCtrl &p = GetListCtrl();
		p.DeleteAllItems();
	}
}


void CStreamsView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF m_clrText2, m_clrBKcolor;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		POSITION pos = this->GetListCtrl().GetFirstSelectedItemPosition();
		int index = this->GetListCtrl().GetNextSelectedItem(pos); //获取鼠标点击的哪一项索引

		if (nItem == index)    //某一项被选中则显示
		{
			m_clrText2 = RGB(255, 255, 255);
			m_clrBKcolor = RGB(49, 106, 197);
		}
		else    //其余的默认如下
		{
			m_clrText2 = RGB(0, 0, 0);
			m_clrBKcolor = RGB(255, 255, 255);
		}
		pLVCD->clrText = m_clrText2;
		pLVCD->clrTextBk = m_clrBKcolor;
	}
}


void CStreamsView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	//POSITION pos = GetListCtrl().GetFirstSelectedItemPosition();
	//int i = 0;
	//if (pos == NULL)
	//{

	//	//未选中

	//}

	//else
	//{
	//	//有选中
	//	while (pos)
	//	{
	//		i++;
	//	    int nItem = GetListCtrl().GetNextSelectedItem(pos);
	//		//      TRACE1("Item %d was selected!/n", nItem);
	//		//      you could do your own processing on nItem here
	//	}
	//}
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	if (pNMItemActivate->iItem == -1)
	{
		pDoc->curStream = 0;
		return;
	}
	int dwIndex = pNMItemActivate->iItem;
	pDoc->curStream = (CSyncStream*)this->GetListCtrl().GetItemData(dwIndex);
	pDoc->AddPacket2PacketView((CSyncStream*)(pDoc->curStream));
	*pResult = 0;
}


int CStreamsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	CString m_ColumnLabelStr[] = { "序号","Time","Source","Destination","Protocol","sum" };
	//表头字段
	CListCtrl& listctrl = GetListCtrl();//获取列表的控件
	DWORD dwStyle = listctrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	// 选中某行使整行高亮（只适用与report 风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;
	dwStyle |= LVS_EX_UNDERLINEHOT;
	listctrl.SetExtendedStyle(dwStyle);//列表风格
	int width[6] = { 30,105,100, 100, 40, 40 };
	for (int i = 0; i < 6; i++)
	{
		listctrl.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // 设置表头
	}
	return 0;
}
