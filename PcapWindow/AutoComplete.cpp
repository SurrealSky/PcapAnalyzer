// AutoComplete.cpp : 实现文件
//

#include "stdafx.h"
#include "AutoComplete.h"
#include<list>
#include"include.h"


// CAutoComplete

IMPLEMENT_DYNAMIC(CAutoComplete, CComboBox)

CAutoComplete::CAutoComplete()
{
	m_redcolor = RGB(0xFF, 0xAF, 0xAF);                      // 红色  
	m_bluecolor = RGB(0xAF, 0xFF, 0xAF);                     // 蓝色  
	m_textcolor = RGB(255, 255, 255);                 // 文本颜色设置为白
	curColor = m_textcolor;
}

CAutoComplete::~CAutoComplete()
{
}


BEGIN_MESSAGE_MAP(CAutoComplete, CComboBox)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CAutoComplete::OnCbnDropdown)
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, &CAutoComplete::OnCbnEditupdate)
	ON_MESSAGE(WM_SHOWDROP, OnShowDropDown)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



// CAutoComplete 消息处理程序


BOOL CAutoComplete::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_CHAR)
	{
		m_bAutoComplete = TRUE;
		int nVirKey = pMsg->wParam;	
		switch (nVirKey)
		{
		case VK_RETURN:
		{
			// 关闭下拉框
			ShowDropDown(FALSE);
			CString strLine;
			GetWindowText(strLine);
			// 回车即选中高亮项(只有匹配时才设置为已选)
			int iSelectedRow = FindString(-1, strLine);
			if (-1 != iSelectedRow)
			{
				SelectString(-1, strLine);
			}
			// 给父窗口发送选项改变的消息 
			WPARAM wParam = MAKELPARAM(GetDlgCtrlID(), CBN_SELCHANGE);
			GetParent()->PostMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
			GetParent()->PostMessage(WM_UPDATEUI, 4, 0);
			break;
		}
		case VK_DELETE:
		case VK_BACK:
			m_bAutoComplete = FALSE;
			break;
		default:
			break;
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

void CAutoComplete::OnCbnDropdown()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}


void CAutoComplete::OnCbnEditupdate()
{
	CString strLine;
	GetWindowText(strLine);
	int iHiLightStart = strLine.GetLength();
	if (strLine.GetLength() == 0)
	{
		ShowDropDown(FALSE);
		SetWindowText(_T(""));
		m_bAutoComplete = TRUE;
		return;
	}
	// 处理删除操作
	if (!m_bAutoComplete)
	{
		m_bAutoComplete = TRUE;
		return;
	}
	// 开始匹配用户输入
	int iSelectedRow = FindString(-1, strLine);
	if (iSelectedRow >= 0)
	{
		// ShowDropDown(TRUE);
		PostMessage(WM_SHOWDROP, 0, 0);
		// 匹配的选项被选中
		PostMessage(CB_SETCURSEL, iSelectedRow, 0);
		// 给父窗口发送选项改变的消息,这样可以保证当输入完整的匹配时,不用回车也触发改变消息
		WPARAM wParam = MAKELPARAM(GetDlgCtrlID(), CBN_SELCHANGE);
		GetParent()->PostMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
		// 高亮自动完成的部分
		PostMessage(CB_SETEDITSEL, 0, MAKELPARAM(iHiLightStart, -1));
	}
	else
	{
		ShowDropDown(FALSE);
		//SetWindowText(strLine);
	}

}

HRESULT CAutoComplete::OnShowDropDown(WPARAM wParam, LPARAM lParam)
{
    ShowDropDown(TRUE);
	return 0;
}


HBRUSH CAutoComplete::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	switch (nCtlColor)
	{
		case CTLCOLOR_EDIT:
		{
			pDC->SetBkColor(curColor);
		}break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CAutoComplete::SetEdtBkColor(COLORREF color)
{
	curColor = color;
}
