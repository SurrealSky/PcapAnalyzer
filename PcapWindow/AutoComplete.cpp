// AutoComplete.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoComplete.h"
#include<list>
#include"include.h"


// CAutoComplete

IMPLEMENT_DYNAMIC(CAutoComplete, CComboBox)

CAutoComplete::CAutoComplete()
{
	m_redcolor = RGB(0xFF, 0xAF, 0xAF);                      // ��ɫ  
	m_bluecolor = RGB(0xAF, 0xFF, 0xAF);                     // ��ɫ  
	m_textcolor = RGB(255, 255, 255);                 // �ı���ɫ����Ϊ��
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



// CAutoComplete ��Ϣ�������


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
			// �ر�������
			ShowDropDown(FALSE);
			CString strLine;
			GetWindowText(strLine);
			// �س���ѡ�и�����(ֻ��ƥ��ʱ������Ϊ��ѡ)
			int iSelectedRow = FindString(-1, strLine);
			if (-1 != iSelectedRow)
			{
				SelectString(-1, strLine);
			}
			// �������ڷ���ѡ��ı����Ϣ 
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
	// ����ɾ������
	if (!m_bAutoComplete)
	{
		m_bAutoComplete = TRUE;
		return;
	}
	// ��ʼƥ���û�����
	int iSelectedRow = FindString(-1, strLine);
	if (iSelectedRow >= 0)
	{
		// ShowDropDown(TRUE);
		PostMessage(WM_SHOWDROP, 0, 0);
		// ƥ���ѡ�ѡ��
		PostMessage(CB_SETCURSEL, iSelectedRow, 0);
		// �������ڷ���ѡ��ı����Ϣ,�������Ա�֤������������ƥ��ʱ,���ûس�Ҳ�����ı���Ϣ
		WPARAM wParam = MAKELPARAM(GetDlgCtrlID(), CBN_SELCHANGE);
		GetParent()->PostMessage(WM_COMMAND, wParam, (LPARAM)m_hWnd);
		// �����Զ���ɵĲ���
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

	// TODO:  �ڴ˸��� DC ���κ�����
	switch (nCtlColor)
	{
		case CTLCOLOR_EDIT:
		{
			pDC->SetBkColor(curColor);
		}break;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CAutoComplete::SetEdtBkColor(COLORREF color)
{
	curColor = color;
}
