#include "stdafx.h"
#include "OwnerDrawMenu.h"

COwnerDrawMenu::COwnerDrawMenu()
{
}


COwnerDrawMenu::~COwnerDrawMenu()
{
}

void COwnerDrawMenu::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CString strText;
	// ��ȡ��ͼ���
	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC); 
	CRect rect(lpDrawItemStruct->rcItem);

	// ѡ��Ч��
	if (lpDrawItemStruct->itemState & ODS_SELECTED)
	{
		// �ڲ˵������Ի���ο�ı�����ɫ 
		pDC->FillSolidRect(rect, OWNER_MENU_HIGH_LIGHT);
		// ѡ����ɫ
		pDC->SetTextColor(OWNER_MENU_HH_COLOR);
	}
	// δѡ��
	else
	{
		pDC->FillSolidRect(rect, OWNER_MENU_BK_COLOR);
		pDC->SetTextColor(OWNER_MENU_TEXT_COLOR);
	}
	pDC->SetBkMode(TRANSPARENT);
	rect.left += 30;

	HFONT ft = CreateFont((rect.bottom - rect.top) * 3 / 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, _T("΢���ź�"));
	HFONT hOldFont = (HFONT)SelectObject(pDC->m_hDC, ft);

	SelectObject(pDC->m_hDC, ft);

	CMenu menu;
	CString str;
	menu.Attach((HMENU)lpDrawItemStruct->hwndItem);
	menu.GetMenuString(lpDrawItemStruct->itemID, str, MF_BYCOMMAND);
	pDC->DrawText(str, rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	menu.Detach();

	SelectObject(pDC->m_hDC, hOldFont);
	DeleteObject(ft);

	return CMenu::DrawItem(lpDrawItemStruct);
}

void COwnerDrawMenu::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct->itemHeight = 27;
	lpMeasureItemStruct->itemWidth = 166;
	return CMenu::MeasureItem(lpMeasureItemStruct);
}
