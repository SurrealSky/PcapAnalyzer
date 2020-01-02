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
	ON_CBN_SELCHANGE(IDD_AUTO_CTRL, &CPacketsView::OnCbnSelchangeCombo)
	ON_NOTIFY(NM_CLICK,IDD_PACKETS_LIST, &CPacketsView::OnPacketsNMClick)
	ON_NOTIFY(NM_CUSTOMDRAW, IDD_PACKETS_LIST, &CPacketsView::OnNMCustomdraw)
	ON_CBN_EDITCHANGE(IDD_AUTO_CTRL, &CPacketsView::OnCbnEditchangeCombo)
	ON_MESSAGE(WM_UPDATEUI, &CPacketsView::OnUpdateui)
END_MESSAGE_MAP()


// CPacketsView ���

#ifdef _DEBUG
void CPacketsView::AssertValid() const
{
	//CFormView::AssertValid();
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
	packets.Create(WS_BORDER | WS_VISIBLE | LVS_REPORT, rect, this, IDD_PACKETS_LIST);
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

void CPacketsView::OnPacketsNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem == -1) return;
	int dwIndex = pNMLV->iItem;
	//�ڼ���
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	CSyncPacket *packet= static_cast<CSyncPacket*>((void*)packets.GetItemData(dwIndex));
	if (packet->_payload.size() > 0)
	{
		std::map<std::string, std::string> result;
		result = pDoc->CACap.PacketAnalysis(packet->_payload, packet->mNetInfo.srcport, packet->mNetInfo.dstport);
		pDoc->Result2AnalysisView(result);
		pDoc->Packet2HexView((CSyncPacket*)(packets.GetItemData(dwIndex)));
	}
	*pResult = 0;
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

void CPacketsView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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

		POSITION pos = packets.GetFirstSelectedItemPosition();
		int index = packets.GetNextSelectedItem(pos); //��ȡ���������һ������

		if (nItem == index)    //ĳһ�ѡ������ʾ
		{
			m_clrText2 = RGB(255, 255, 255);
			m_clrBKcolor = RGB(49, 106, 197);
		}
		else    //�����Ĭ������
		{
			m_clrText2 = RGB(0, 0, 0);
			m_clrBKcolor = RGB(255, 255, 255);
		}
		pLVCD->clrText = m_clrText2;
		pLVCD->clrTextBk = m_clrBKcolor;
	}
}


afx_msg LRESULT CPacketsView::OnUpdateui(WPARAM wParam, LPARAM lParam)
{
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	switch (wParam)
	{
		case 0x0004:
		{
			CString exp = "";
			dfilterEdt.GetWindowTextA(exp);
			std::string strExp;
			strExp = exp;
			if (pDoc->curStream != 0&& pDoc->curStream->GetCount()>0)
			{
				packets.DeleteAllItems();
				//mAnalysisResult.DeleteAllItems();
				//mHexView.SetData(0, 0);
				std::list<CSyncPacket>::iterator iter;
				for (iter = pDoc->curStream->GetBegin(); iter != pDoc->curStream->GetEnd(); iter++)
				{
					AddPacket2UI(&(*iter), strExp);
				}
			}
		}break;
	}
	return 0;
}


BOOL CPacketsView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		dfilterEdt.PostMessage(WM_CHAR, (WPARAM)VK_RETURN, 0);
		return TRUE;
	}
	return CFormView::PreTranslateMessage(pMsg);
}


void CPacketsView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (lHint == WM_PACKETVIEW_ADDPACKET)
	{
		CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
		CSyncStream *stream = static_cast<CSyncStream*>((void*)pHint);
		if (stream)
		{
			CListCtrl &p = packets;
			p.SetRedraw(FALSE);//�����б������˸
			p.DeleteAllItems();
			std::list<CSyncPacket>::iterator iter;
			for (iter = stream->GetBegin(); iter != stream->GetEnd(); iter++)
			{
				CString strExp;
				dfilterEdt.GetWindowTextA(strExp);
				AddPacket2UI(&*iter, strExp.GetBuffer(0));
			}
			p.SetRedraw(TRUE);
		}
	}
	else if (lHint == WM_VIEW_CLEAR)
	{
		packets.DeleteAllItems();
	}
}
