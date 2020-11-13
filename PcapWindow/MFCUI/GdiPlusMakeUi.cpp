#include "stdafx.h"
#include "GdiPlusMakeUi.h"

GdiplusStartupInput gdiplusStartupInputUi;
ULONG_PTR gdiplusTokenUi;


CGdiPlusMakeUi::CGdiPlusMakeUi()
{

}

CGdiPlusMakeUi::~CGdiPlusMakeUi()
{

}

// GDI+��ʼ��
void CGdiPlusMakeUi::CGdiPlusMakeUiInit()
{
	// GDI�����ʼ��
	GdiplusStartup(&gdiplusTokenUi, &gdiplusStartupInputUi, NULL);
}

// ���Ʊ�Ե�߿�
void DrawWindowRectUi(HDC & hdc, CRect rt)
{
	CPen pen(PS_SOLID, 1, RGB(0, 139, 160));
	SelectObject(hdc, pen);
	MoveToEx(hdc, rt.left, rt.top, NULL);
	LineTo(hdc, rt.left, rt.bottom - 1);
	LineTo(hdc, rt.right - 1, rt.bottom - 1);
	LineTo(hdc, rt.right - 1, rt.top);
	LineTo(hdc, rt.left, rt.top);

	CRect rm = rt;
	rm.top = rm.bottom - 5;
	CBrush bhs(RGB(0, 139, 160));
	FillRect(hdc, rm, bhs);
}
