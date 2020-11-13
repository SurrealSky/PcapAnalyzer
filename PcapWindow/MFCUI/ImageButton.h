#pragma once

#include "GdiPlusMakeUi.h"

class CImageButton : public CButton
{
public:
	CImageButton();
	void SetButtonImage(CStringW strNorPath, CStringW strHovPath, CStringW strPushPath);
	~CImageButton();

	Image * m_pNormalImage;
	Image * m_pHoverImage;
	Image * m_pDownImage;

	HBRUSH m_hBackBrush;

	BOOL m_MouseOnButton;

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	// ���ƽ���
	void DrawButton(LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
};