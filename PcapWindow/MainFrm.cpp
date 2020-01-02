
// MainFrm.cpp : CMainFrame ���ʵ��
//

#include "stdafx.h"
#include "PcapWindow.h"

#include "MainFrm.h"
#include"include.h"
#include"resource.h"
#include"PcapWindowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_CAPTURE_START, &CMainFrame::OnCaptureStart)
	ON_COMMAND(ID_CAPTURE_STOP, &CMainFrame::OnCaptureStop)
	ON_COMMAND(ID_CAPTURE_RESTART, &CMainFrame::OnCaptureRestart)
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_WM_SETTINGCHANGE()
	ON_MESSAGE(WM_HEXVIEW_PACKET, &CMainFrame::OnHexviewPacket)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE_START, &CMainFrame::OnUpdateCaptureStart)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE_RESTART, &CMainFrame::OnUpdateCaptureRestart)
	ON_UPDATE_COMMAND_UI(ID_CAPTURE_STOP, &CMainFrame::OnUpdateCaptureStop)
	ON_MESSAGE(WM_HEXVIEW_CLEAR, &CMainFrame::OnHexviewClear)
	ON_MESSAGE(WM_HEXVIEW_PAIR, &CMainFrame::OnHexviewPair)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ״̬��ָʾ��
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // ����������ʽ...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // ����Ϊ FALSE �Ὣ�رհ�ť������ѡ�������Ҳ�
	mdiTabParams.m_bTabIcons = TRUE;    // ����Ϊ TRUE ���� MDI ѡ��������ĵ�ͼ��
	mdiTabParams.m_bAutoColor = TRUE;    // ����Ϊ FALSE ������ MDI ѡ����Զ���ɫ
	mdiTabParams.m_bDocumentMenu = TRUE; // ��ѡ�������ұ�Ե�����ĵ��˵�
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("δ�ܴ����˵���\n");
		return -1;      // δ�ܴ���
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// ��ֹ�˵����ڼ���ʱ��ý���
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//ϵͳĬ�Ϲ�����
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME_256))
	{
		TRACE0("δ�ܴ���������\n");
		return -1;      // δ�ܴ���
	}

	LoadUI();

	//CString strToolBarName;
	//bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	// ȥ�����������Ҳ���Զ��尴ť
	//m_wndToolBar.EnableCustomizeButton(FALSE, ID_VIEW_CUSTOMIZE, strCustomize);

	// �����û�����Ĺ���������: 
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: �������ϣ���������Ͳ˵�����ͣ������ɾ��������
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);	//��ֹ�˵�������
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);	//��ֹ����������
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// ���� Visual Studio 2005 ��ʽͣ��������Ϊ
	CDockingManager::SetDockingMode(DT_SMART);
	// ���� Visual Studio 2005 ��ʽͣ�������Զ�������Ϊ
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// ���ز˵���ͼ��(�����κα�׼��������): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// ����ͣ������
	if (!CreateDockingWindows())
	{
		TRACE0("δ�ܴ���ͣ������\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	//m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	//m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	//DockPane(&m_wndProperties);

	// ���ڳ־�ֵ�����Ӿ�����������ʽ
	OnApplicationLook(theApp.m_nAppLook);

	// ������ǿ�Ĵ��ڹ���Ի���
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// ���ù�������ͣ�����ڲ˵��滻(���β˵� �Զ��� ��)
	EnablePaneMenu(TRUE, /*ID_VIEW_CUSTOMIZE*/0, strCustomize, ID_VIEW_TOOLBAR);

	// ���ÿ���(��ס Alt �϶�)�������Զ���
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// �����û�����Ĺ�����ͼ��
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// ���ò˵����Ի�(���ʹ�õ�����)
	// TODO: �������Լ��Ļ������ȷ��ÿ�������˵�������һ���������
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_WINDOWS_7);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	// ���ĵ�����Ӧ�ó��������ڴ��ڱ������ϵ�˳����н�������
	// ���Ľ��������Ŀ����ԣ���Ϊ��ʾ���ĵ�����������ͼ��
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

CString CMainFrame::SplicFullFilePath(CString strExeModuleName)
{
	// ��ȡ��ǰ·��
	// ׼��д�ļ�
	CHAR strPath[MAX_PATH + 1] = { 0 };
	CHAR * pTempPath;
	GetModuleFileName(NULL, strPath, MAX_PATH);
	pTempPath = strPath;
	while (strchr(pTempPath, '\\'))
	{
		pTempPath = strchr(pTempPath, '\\');
		pTempPath++;
	}
	*pTempPath = 0;

	CString strPathName;
	strPathName += strPath + strExeModuleName;
	return strPathName;
}

void CMainFrame::LoadUI()
{
	int index = 0;
	RECT rect;
	int width = 20;
	int offset = 0;

	////����ָ��������Ŀ�Ȳ���ȡ�µ�����  20�ǿ��
	////index = m_wndToolBar.CommandToIndex(ID_NET_DEVS);
	//index = 3;
	//m_wndToolBar.SetButtonInfo(index, ID_PLUGINS, TBBS_SEPARATOR, 1);
	//m_wndToolBar.GetItemRect(index, &rect);
	//rect.right = rect.left + width * 4;
	//offset = rect.right;
	//rect.bottom += 100;
	////rect.top += 2;
	////rect.bottom -= 5;
	//// ��������ʾ�ؼ�
	//if (!m_wndPlugins.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, rect,
	//	&m_wndToolBar, ID_PLUGINS))
	//{
	//	TRACE0("Failed to create combo-box\n");
	//	return;
	//}
	//m_wndPlugins.ShowWindow(SW_HIDE);

	//����ָ��������Ŀ�Ȳ���ȡ�µ�����  1�ǿ��
	//index = m_wndToolBar.CommandToIndex(ID_NET_DEVS);
	index = 3;
	m_wndToolBar.SetButtonInfo(index, ID_NET_DEVS, TBBS_SEPARATOR, 1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.right = rect.left + width * 15;
	offset = rect.right;
	rect.bottom += 100;
	//rect.top += 2;
	rect.bottom -= 5;
	if (!m_wndDevs.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, rect,
		&m_wndToolBar, ID_NET_DEVS))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	m_wndDevs.ShowWindow(SW_HIDE);
	//������������
	std::vector<NetCardInfo> devs;
	PcapAnalyzer::LoadNetDevs(devs);
	std::vector<NetCardInfo>::const_iterator iter = devs.begin();
	for (; iter != devs.end(); iter++)
	{
		m_wndDevs.InsertString(m_wndDevs.GetCount(), iter->description.c_str());
	}
	m_wndDevs.SetCurSel(0);


	//����ָ��������Ŀ�Ȳ���ȡ�µ�����  20�ǿ��
	//index=m_wndToolBar.GetCount();
	index = 4;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_START, TBBS_SEPARATOR, 1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// ��������ʾ�ؼ�
	if (!m_StartCapture.Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | WS_CLIPCHILDREN, rect,
		&m_wndToolBar, ID_CAPTURE_START))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// ���ð�ť1ͼƬ
	
	m_StartCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-start.png"), SplicFullFilePath("Res\\Button\\x-capture-start-white.png"));
	m_StartCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//�O�ð��o�@ʾ��͸���NĤ
	m_StartCapture.ShowWindow(SW_HIDE);
	m_StartCapture.SizeToContent();

	//����ָ��������Ŀ�Ȳ���ȡ�µ�����  80�ǿ��
	//index = m_wndToolBar.CommandToIndex(ID_CAPTURE_STOP);
	index = 5;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_STOP, TBBS_SEPARATOR, 1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// ��������ʾ�ؼ�
	if (!m_StopCapture.Create("", WS_CHILD | WS_VISIBLE, rect,
		&m_wndToolBar, ID_CAPTURE_STOP))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// ���ð�ť1ͼƬ
	m_StopCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-stop.png"), SplicFullFilePath("Res\\Button\\x-capture-stop-white.png"));
	m_StopCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//�O�ð��o�@ʾ��͸���NĤ
	m_StopCapture.ShowWindow(SW_HIDE);

	//����ָ��������Ŀ�Ȳ���ȡ�µ�����  80�ǿ��
	//index = m_wndToolBar.CommandToIndex(ID_CAPTURE_RESTART);
	index = 6;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_RESTART, TBBS_SEPARATOR,1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// ��������ʾ�ؼ�
	if (!m_RestartCapture.Create("", WS_CHILD | WS_VISIBLE, rect,
		&m_wndToolBar, ID_CAPTURE_RESTART))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// ���ð�ť1ͼƬ
	m_RestartCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-restart.png"), SplicFullFilePath("Res\\Button\\x-capture-restart-turn1.png"));
	m_RestartCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//�O�ð��o�@ʾ��͸���NĤ
	m_RestartCapture.ShowWindow(SW_HIDE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// ��������ͼ
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ���������ͼ������\n");
	//	return FALSE; // δ�ܴ���
	//}

	// �����ļ���ͼ
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ������ļ���ͼ������\n");
		return FALSE; // δ�ܴ���
	}

	// �����������
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("δ�ܴ����������\n");
		return FALSE; // δ�ܴ���
	}

	// �������Դ���
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("δ�ܴ��������ԡ�����\n");
	//	return FALSE; // δ�ܴ���
	//}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	//HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	//HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	//m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame ���

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame ��Ϣ�������

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* ɨ��˵�*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	/*CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);*/
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// ���ཫִ�������Ĺ���

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// Ϊ�����û������������Զ��尴ť
	//BOOL bNameValid;
	//CString strCustomize;
	//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);

	//for (int i = 0; i < iMaxUserToolbars; i ++)
	//{
	//	CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
	//	if (pUserToolbar != NULL)
	//	{
	//		pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	//	}
	//}
	//������������ui���ƽ������ܽ�������UI����
	//m_wndPlugins.ShowWindow(SW_SHOW);
	m_wndDevs.ShowWindow(SW_SHOW);
	m_StartCapture.ShowWindow(SW_SHOW);
	m_StopCapture.ShowWindow(SW_SHOW);
	m_RestartCapture.ShowWindow(SW_SHOW);
	return TRUE;
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

afx_msg LRESULT CMainFrame::OnHexviewPacket(WPARAM wParam, LPARAM lParam)
{
	CSyncPacket *packet = static_cast<CSyncPacket*>((void*)wParam);
	m_wndOutput.Data2HexView(packet->_payload.size(), (STu8*)(packet->_payload.contents()));
	return 0;
}

afx_msg void CMainFrame::OnCaptureStart()
{
	PostMessage(WM_COMMAND, ID_FILE_NEW, 0);
}

afx_msg void CMainFrame::OnCaptureStop()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	CPcapWindowDoc *pDoc = (CPcapWindowDoc *)pActiveChild->GetActiveDocument();
	pDoc->OnCaptureStop();
}

afx_msg void CMainFrame::OnCaptureRestart()
{
	CMDIChildWnd* pActiveChild = MDIGetActive();
	CPcapWindowDoc *pDoc = (CPcapWindowDoc *)pActiveChild->GetActiveDocument();
	pDoc->OnCaptureRestart();
}

void CMainFrame::OnUpdateCaptureStart(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild)
	{
		CPcapWindowDoc *pDoc = (CPcapWindowDoc *)pActiveChild->GetActiveDocument();
		if (pDoc->CACap.IsSniffing())
		{
			pCmdUI->Enable(FALSE);
			return;
		}
	}
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateCaptureStop(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild)
	{
		CPcapWindowDoc *pDoc = (CPcapWindowDoc *)pActiveChild->GetActiveDocument();
		if (pDoc->CACap.IsSniffing())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CMainFrame::OnUpdateCaptureRestart(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild)
	{
		CPcapWindowDoc *pDoc = (CPcapWindowDoc *)pActiveChild->GetActiveDocument();
		if (pDoc->CACap.IsSniffing())
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

afx_msg LRESULT CMainFrame::OnHexviewClear(WPARAM wParam, LPARAM lParam)
{
	m_wndOutput.ClearHexView();
	return 0;
}


afx_msg LRESULT CMainFrame::OnHexviewPair(WPARAM wParam, LPARAM lParam)
{
	std::pair<std::string,std::string> *element = static_cast<std::pair<std::string, std::string>*>((void*)wParam);
	m_wndOutput.Data2HexView(element->second.size(), (STu8*)(element->second.c_str()));
	return 0;
}
