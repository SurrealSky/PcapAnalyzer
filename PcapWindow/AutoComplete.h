#pragma once


// CAutoComplete

class CAutoComplete : public CComboBox
{
	DECLARE_DYNAMIC(CAutoComplete)

public:
	CAutoComplete();
	virtual ~CAutoComplete();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnDropdown();
	afx_msg void OnCbnEditupdate();
	afx_msg HRESULT OnShowDropDown(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	BOOL m_bAutoComplete;
public:
	COLORREF m_redcolor, m_bluecolor, m_textcolor;
public:
	void SetEdtBkColor(COLORREF);
private:
	COLORREF curColor;
};


