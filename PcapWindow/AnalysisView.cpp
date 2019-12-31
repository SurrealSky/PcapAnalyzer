// AnalysisView.cpp : 实现文件
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


// CAnalysisView 诊断

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


// CAnalysisView 消息处理程序


void CAnalysisView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();


	// TODO: 在此添加专用代码和/或调用基类
	CString m_ColumnLabelStr[] = { "序号","字段","值","Hex String" };

	//表头字段

	CListCtrl& listctrl = GetListCtrl();//获取列表的控件

	DWORD dwStyle = listctrl.GetExtendedStyle();

	dwStyle |= LVS_EX_FULLROWSELECT;

	// 选中某行使整行高亮（只适用与report 风格的listctrl）

	dwStyle |= LVS_EX_GRIDLINES;

	dwStyle |= LVS_EX_UNDERLINEHOT;

	listctrl.SetExtendedStyle(dwStyle);//列表风格

	int width[6] = { 30,105,100, 100 };

	for (int i = 0; i < 4; i++)
	{
		listctrl.InsertColumn(i, m_ColumnLabelStr[i], LVCFMT_LEFT, width[i]); // 设置表头
	}
}


BOOL CAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style = cs.style | LVS_REPORT| LVS_SINGLESEL;// 设置成报告列表的显示形式
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


void CAnalysisView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加控件通知处理程序代码
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iItem == -1) return;
	int dwIndex = pNMLV->iItem;
	//第几项
	CPcapWindowDoc *pDoc = static_cast<CPcapWindowDoc*>(this->GetDocument());
	pDoc->curElement = (void*)GetListCtrl().GetItemData(dwIndex);
	pDoc->Result2HexView((std::pair<std::string,std::string>*)(pDoc->curElement));
	*pResult = 0;
}
