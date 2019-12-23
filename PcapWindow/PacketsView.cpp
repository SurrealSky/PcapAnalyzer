// PacketsView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PcapWindow.h"
#include"PcapWindowDoc.h"
#include "PacketsView.h"


// CPacketsView

IMPLEMENT_DYNCREATE(CPacketsView, CFormView)

CPacketsView::CPacketsView()
	: CFormView(IDD_PACKETSVIEW)
{

}

CPacketsView::~CPacketsView()
{
}

void CPacketsView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPacketsView, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_STREAMVIEW_ADDPACKET, &CPacketsView::OnStreamviewAddpacket)
	ON_CBN_SELCHANGE(IDD_AUTO_CTRL, &CPacketsView::OnCbnSelchangeCombo)
	ON_CBN_EDITCHANGE(IDD_AUTO_CTRL, &CPacketsView::OnCbnEditchangeCombo)
END_MESSAGE_MAP()


// CPacketsView ���

#ifdef _DEBUG
void CPacketsView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPacketsView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CPacketsView ��Ϣ�������


int CPacketsView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CRect rect;
	GetClientRect(&rect);
	//rect.bottom -= 250;
	packets.Create(WS_BORDER | WS_VISIBLE | LVS_REPORT, rect, this, -1);
	DWORD dwStyle = 0;
	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	//dwStyle |= LVS_SHOWSELALWAYS;
	dwStyle |= LVS_EX_UNDERLINEHOT;
	packets.SetExtendedStyle(dwStyle);
	CString m_ColumnLabelStr[] = { "���","Time","SrcPort","DstPort","Length" };
	int width[6] = { 30,105,80, 80, 40 };
	for (int i = 0; i < 5; i++)
	{
		packets.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // ���ñ�ͷ
	}
	packets.ShowWindow(SW_SHOW);

	//���˱��ʽ��
	GetClientRect(&rect);
	//rect.top = rect.top-20;
	//rect.right = rect.right - 20;
	dfilterEdt.Create(WS_CHILD | WS_VISIBLE|WS_BORDER| CBS_DROPDOWN| CBS_HASSTRINGS| WS_VSCROLL, rect, this, IDD_AUTO_CTRL);
	dfilterEdt.SetDroppedWidth(100);
	dfilterEdt.ShowWindow(SW_SHOW);
	dfilterEdt.BringWindowToTop();
	return 0;
}

void CPacketsView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CRect rect;
	//�������ݰ��б�ؼ�λ��
	GetClientRect(&rect);
	rect.bottom -= 30;
	packets.MoveWindow(rect, TRUE);
	//������ʾ���˿ؼ�λ��
	GetClientRect(&rect);
	rect.top = rect.bottom - 25;
	rect.bottom = rect.bottom - 5;
	rect.right = rect.right - 20;
	dfilterEdt.MoveWindow(rect, TRUE);

	//ȥ����ͼ�Ĺ�����
	CSize size;
	GetClientRect(&rect);
	size.cx = rect.right - rect.left;
	size.cy = rect.bottom - rect.top;
	SetScrollSizes(MM_HIMETRIC, size); // ��CScrollView�Ĵ�С����Ϊ��ǰ�ͻ�����С
}

afx_msg LRESULT CPacketsView::OnStreamviewAddpacket(WPARAM wParam, LPARAM lParam)
{
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CSyncStream *stream = static_cast<CSyncStream*>((void*)wParam);
	if (stream)
	{
		CListCtrl &p = packets;
		p.DeleteAllItems();
		std::list<CSyncPacket>::iterator iter;
		for (iter = stream->GetBegin(); iter != stream->GetEnd(); iter++)
		{
			CString strExp;
			dfilterEdt.GetWindowTextA(strExp);
			AddPacket2UI(&*iter, strExp.GetBuffer(0));
		}
	}
	return 0;
}

void CPacketsView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	//���ع��˱��ʽʾ��
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	std::list<std::string> exps = pDoc->CACap.LoadExpression();
	std::list<std::string>::iterator iter = exps.begin();
	for (; iter != exps.end(); iter++)
	{
		dfilterEdt.InsertString(0, iter->c_str());
	}
	dfilterEdt.BringWindowToTop();
}

void CPacketsView::OnCbnSelchangeCombo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CString exp = "";
	dfilterEdt.GetWindowTextA(exp);
	if (exp.GetLength() > 0)
	{
		bool isValid = pDoc->CACap.IsValidateeExpression(exp.GetBuffer(0));
		if (isValid)
		{
			dfilterEdt.SetEdtBkColor(dfilterEdt.m_bluecolor);
		}
		else
		{
			dfilterEdt.SetEdtBkColor(dfilterEdt.m_redcolor);
		}
	}
	else
		dfilterEdt.SetEdtBkColor(dfilterEdt.m_textcolor);
	dfilterEdt.Invalidate(TRUE);
}

void CPacketsView::OnCbnEditchangeCombo()
{
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CString exp = "";
	dfilterEdt.GetWindowTextA(exp);
	if (exp.GetLength() > 0)
	{
		bool isValid = pDoc->CACap.IsValidateeExpression(exp.GetBuffer(0));
		if (isValid)
		{
			dfilterEdt.SetEdtBkColor(dfilterEdt.m_bluecolor);
		}
		else
		{
			dfilterEdt.SetEdtBkColor(dfilterEdt.m_redcolor);
		}
	}
	else
		dfilterEdt.SetEdtBkColor(dfilterEdt.m_textcolor);
	dfilterEdt.Invalidate(TRUE);
}

void CPacketsView::AddPacket2UI(CSyncPacket *pack, std::string strExp)
{
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	bool isFilter = pDoc->CACap.ForDisplayFilter(pack->_payload.contents(), pack->_payload.size(), strExp);
	if (!isFilter) return;

	STu32 count = packets.GetItemCount();

	CString item = "";
	item.Format("%d", count + 1);
	packets.InsertItem(count, item);

	packets.SetItemText(count, 1, pDoc->CACap.FormatTime(pack->time).c_str());

	item.Format("%d", pack->mNetInfo.srcport);
	packets.SetItemText(count, 2, item);

	item.Format("%d", pack->mNetInfo.dstport);
	packets.SetItemText(count, 3, item);

	item.Format("%d", pack->_payload.size());
	packets.SetItemText(count, 4, item);

	packets.SetItemData(count, (DWORD_PTR)(pack));
	packets.EnsureVisible(count, FALSE);
}
