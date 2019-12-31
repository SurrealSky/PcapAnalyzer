// AnalysisView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "AnalysisView.h"
#include"include.h"
#include"PcapWindowDoc.h"


// CAnalysisView

IMPLEMENT_DYNCREATE(CAnalysisView, CListView)

CAnalysisView::CAnalysisView()
{

}

CAnalysisView::~CAnalysisView()
{
}

BEGIN_MESSAGE_MAP(CAnalysisView, CListView)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CAnalysisView::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(NM_CLICK, &CAnalysisView::OnNMClick)
END_MESSAGE_MAP()


// CAnalysisView ���

#ifdef _DEBUG
void CAnalysisView::AssertValid() const
{
	//CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CAnalysisView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAnalysisView ��Ϣ�������


void CAnalysisView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: �ڴ����ר�ô����/����û���
	CString m_ColumnLabelStr[] = { "���","�ֶ�","ֵ","Hex String" };

	//��ͷ�ֶ�

	CListCtrl& listctrl = GetListCtrl();//��ȡ�б�Ŀؼ�

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// ѡ��ĳ��ʹ���и�����ֻ������report ����listctrl��

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//�б���

	int width[6] = { 30,105,100, 100 };

	for (int i = 0; i < 4; i++)
	{
		listctrl.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // ���ñ�ͷ
	}
}


BOOL CAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style = cs.style | LVS_REPORT| LVS_SINGLESEL;// ���óɱ����б����ʾ��ʽ
	return CListView::PreCreateWindow(cs);
}


void CAnalysisView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
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
		int index = this->GetListCtrl().GetNextSelectedItem(pos); //��ȡ���������һ������

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


void CAnalysisView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (lHint == WM_ANALYSISVIEW_MAP)
	{
		CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
		CListCtrl &p = GetListCtrl();
		p.DeleteAllItems();
		std::map<std::string, std::string>::const_iterator iter = pDoc->curResult.begin();
		STu32 index = 0;
		for (; iter != pDoc->curResult.end(); iter++)
		{
			CString item = "";
			item.Format("%d", index + 1);
			p.InsertItem(index, item);

			p.SetItemText(index, 1, iter->first.c_str());

			p.SetItemText(index, 2, iter->second.c_str());

			SurrealHex::HexConvert mHex;
			p.SetItemText(index, 3, mHex.HexToStr((BYTE*)(iter->second.c_str()), iter->second.size()).c_str());

			p.SetItemData(index, (DWORD_PTR)&(*iter));
			index++;
		}
	}
	else if (lHint == WM_VIEW_CLEAR)
	{
		CListCtrl &p = GetListCtrl();
		p.DeleteAllItems();
	}
}


void CAnalysisView::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem == -1) return;
	int dwIndex = pNMLV->iItem;
	//�ڼ���
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	pDoc->curElement = (void*)GetListCtrl().GetItemData(dwIndex);
	pDoc->Result2HexView((std::pair<std::string,std::string>*)(pDoc->curElement));
	*pResult = 0;
}
