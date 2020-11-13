#include "stdafx.h"
#include "OwnerDrawTabList.h"

//#include "..//GameSet//GameSet//RuleSetDlg2.h"

COwnerDrawTabList::COwnerDrawTabList()
{
	m_nSelectIndex = 0;
	m_pParent = NULL;
}


COwnerDrawTabList::~COwnerDrawTabList()
{

}

void COwnerDrawTabList::SetParentCwnd(CWnd * pWnd)
{
	m_pParent = pWnd;
}

void COwnerDrawTabList::SetTabTitle(vector<CString> strVecTabTitle)
{
	// ����tab�б�
	m_strVecTabTitle.assign(strVecTabTitle.begin(), strVecTabTitle.end());
	Invalidate();
}

void COwnerDrawTabList::SetSelectIndex(int index)
{
	m_nSelectIndex = index;
	// ���ø����ˢ����
	//((CRuleSetDlg2 *)m_pParent)->ShowShowHideCtl(m_nSelectIndex);
}

int COwnerDrawTabList::GetSelectIndex()
{
	return m_nSelectIndex;
}

void COwnerDrawTabList::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

}

BEGIN_MESSAGE_MAP(COwnerDrawTabList, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


void COwnerDrawTabList::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CStatic::OnPaint()

	CBrush bh(OWNER_COLOR_BACKGROUND_STC);
	CBrush bhh(OWNER_COLOR_HIGHLIGHT_STC);

	CDC* pDC = CDC::FromHandle(dc.m_hDC);

	CRect itemRect;
	GetClientRect(&itemRect);
	pDC->SetBkMode(TRANSPARENT);

	// ׼������
	HFONT ft = CreateFont((itemRect.bottom - itemRect.top) * 3 / 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("΢���ź�"));
	HFONT hOldFont = (HFONT)SelectObject(pDC->m_hDC, ft);

	for (int i = 0; i < (int)m_strVecTabTitle.size(); i++)
	{
		RECT rt = itemRect;
		rt.left = i * ((itemRect.right - itemRect.left) / m_strVecTabTitle.size());
		rt.right = (i + 1) * ((itemRect.right - itemRect.left) / m_strVecTabTitle.size());
		pDC->FillRect(&rt, i == m_nSelectIndex ? &bh : &bhh);
		pDC->SetTextColor(i == m_nSelectIndex ? OWNER_COLOR_HIGHLIGHT_STC : OWNER_COLOR_BACKGROUND_STC);
		pDC->DrawText(m_strVecTabTitle[i], m_strVecTabTitle[i].GetLength(), &rt, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	}

	SelectObject(pDC->m_hDC, hOldFont);
	DeleteObject(ft);
}

void COwnerDrawTabList::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect itemRect;
	GetClientRect(&itemRect);

	// ȷ�����������
	for (int i = 0; i < (int)m_strVecTabTitle.size(); i++)
	{
		RECT rt = itemRect;
		rt.left = i * ((itemRect.right - itemRect.left) / m_strVecTabTitle.size());
		rt.right = (i + 1) * ((itemRect.right - itemRect.left) / m_strVecTabTitle.size());

		if (PtInRect(&rt, point))
		{
			m_nSelectIndex = i;
			Invalidate();

			// ���ø����ˢ����
			//((CRuleSetDlg2 *)m_pParent)->ShowShowHideCtl(i);
		}
	}
	CStatic::OnLButtonDown(nFlags, point);
}
