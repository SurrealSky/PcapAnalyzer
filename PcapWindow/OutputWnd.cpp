
#include "stdafx.h"

#include "OutputWnd.h"
#include "Resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutputBar

COutputWnd::COutputWnd()
{
}

COutputWnd::~COutputWnd()
{
}

BEGIN_MESSAGE_MAP(COutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// ����ѡ�����: 
	if (!m_wndTabs.Create(CMFCTabCtrl::STYLE_FLAT, rectDummy, this, 1))
	{
		TRACE0("δ�ܴ������ѡ�����\n");
		return -1;      // δ�ܴ���
	}

	// �����������: 
	const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;

	if (!m_wndOutputText.Create(dwStyle, rectDummy, &m_wndTabs, 2) ||
		!m_wndOutputHex.Create("SoftCircuitsHexEdit","SoftCircuitsHexEdit", WS_CHILD, rectDummy,&m_wndTabs,3))
	{
		TRACE0("δ�ܴ����������\n");
		return -1;      // δ�ܴ���
	}
	//m_wndOutputHex.SetWideView(TRUE);
	//m_wndOutputHex.SetReadOnly(TRUE);
	//BYTE buffer[0x100] = { 0xff };
	//m_wndOutputHex.SetData(sizeof(buffer), buffer);

	UpdateFonts();

	CString strTabName;
	BOOL bNameValid;

	// ���б��ڸ��ӵ�ѡ�: 
	bNameValid = strTabName.LoadString(IDS_TEXT_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputText, strTabName, (UINT)0);
	bNameValid = strTabName.LoadString(IDS_HEX_TAB);
	ASSERT(bNameValid);
	m_wndTabs.AddTab(&m_wndOutputHex, strTabName, (UINT)1);

	// ʹ��һЩ�����ı���д���ѡ�(���踴������)
	FillBuildWindow();
	FillDebugWindow();

	return 0;
}

void COutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������: 
	m_wndTabs.SetWindowPos (NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}

void COutputWnd::AdjustHorzScroll(CListBox& wndListBox)
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&afxGlobalData.fontRegular);

	int cxExtentMax = 0;

	for (int i = 0; i < wndListBox.GetCount(); i ++)
	{
		CString strItem;
		wndListBox.GetText(i, strItem);

		cxExtentMax = max(cxExtentMax, (int)dc.GetTextExtent(strItem).cx);
	}

	wndListBox.SetHorizontalExtent(cxExtentMax);
	dc.SelectObject(pOldFont);
}

void COutputWnd::FillBuildWindow()
{
	m_wndOutputText.AddString(_T("�����������ʾ�ڴ˴���"));
	m_wndOutputText.AddString(_T("�������ʾ���б���ͼ������"));
	m_wndOutputText.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::FillDebugWindow()
{
	//m_wndOutputHex.AddString(_T("�����������ʾ�ڴ˴���"));
	//m_wndOutputHex.AddString(_T("�������ʾ���б���ͼ������"));
	//m_wndOutputHex.AddString(_T("�������Ը�����Ҫ��������ʾ��ʽ..."));
}

void COutputWnd::UpdateFonts()
{
	m_wndOutputText.SetFont(&afxGlobalData.fontRegular);
	//m_wndOutputHex.SetFont(&afxGlobalData.fontRegular);
}

void COutputWnd::Data2HexView(STu64 length, STu8 *data)
{
	m_wndTabs.SetActiveTab(1);
	//m_wndOutputHex.SetData(length, data);
}

void COutputWnd::ClearHexView()
{
	m_wndTabs.SetActiveTab(0);
	//m_wndOutputHex.SetData(0, 0);
}

/////////////////////////////////////////////////////////////////////////////
// COutputList1

COutputList::COutputList()
{
}

COutputList::~COutputList()
{
}

BEGIN_MESSAGE_MAP(COutputList, CListBox)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_COMMAND(ID_VIEW_OUTPUTWND, OnViewOutput)
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// COutputList ��Ϣ�������

void COutputList::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	CMenu menu;
	menu.LoadMenu(IDR_OUTPUT_POPUP);

	CMenu* pSumMenu = menu.GetSubMenu(0);

	if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CMDIFrameWndEx)))
	{
		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

		if (!pPopupMenu->Create(this, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CMDIFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	SetFocus();
}

void COutputList::OnEditCopy()
{
	MessageBox(_T("�������"));
}

void COutputList::OnEditClear()
{
	MessageBox(_T("������"));
}

void COutputList::OnViewOutput()
{
	CDockablePane* pParentBar = DYNAMIC_DOWNCAST(CDockablePane, GetOwner());
	CMDIFrameWndEx* pMainFrame = DYNAMIC_DOWNCAST(CMDIFrameWndEx, GetTopLevelFrame());

	if (pMainFrame != NULL && pParentBar != NULL)
	{
		pMainFrame->SetFocus();
		pMainFrame->ShowPane(pParentBar, FALSE, FALSE, FALSE);
		pMainFrame->RecalcLayout();

	}
}
