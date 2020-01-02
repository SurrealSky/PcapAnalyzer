
// MainFrm.cpp : CMainFrame 类的实现
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
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
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
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // 其他可用样式...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // 设置为 FALSE 会将关闭按钮放置在选项卡区域的右侧
	mdiTabParams.m_bTabIcons = TRUE;    // 设置为 TRUE 将在 MDI 选项卡上启用文档图标
	mdiTabParams.m_bAutoColor = TRUE;    // 设置为 FALSE 将禁用 MDI 选项卡的自动着色
	mdiTabParams.m_bDocumentMenu = TRUE; // 在选项卡区域的右边缘启用文档菜单
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//系统默认工具栏
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME_256))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
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
	// 去掉工具栏最右侧的自定义按钮
	//m_wndToolBar.EnableCustomizeButton(FALSE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作: 
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);	//禁止菜单栏浮动
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);	//禁止工具栏浮动
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 加载菜单项图像(不在任何标准工具栏上): 
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
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

	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	// 启用增强的窗口管理对话框
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// 启用工具栏和停靠窗口菜单替换(屏蔽菜单 自定义 项)
	EnablePaneMenu(TRUE, /*ID_VIEW_CUSTOMIZE*/0, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	//CMFCToolBar::EnableQuickCustomization();

	//if (CMFCToolBar::GetUserImages() == NULL)
	//{
	//	// 加载用户定义的工具栏图像
	//	if (m_UserImages.Load(_T(".\\UserImages.bmp")))
	//	{
	//		CMFCToolBar::SetUserImages(&m_UserImages);
	//	}
	//}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
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

	// 将文档名和应用程序名称在窗口标题栏上的顺序进行交换。这
	// 将改进任务栏的可用性，因为显示的文档名带有缩略图。
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

CString CMainFrame::SplicFullFilePath(CString strExeModuleName)
{
	// 提取当前路径
	// 准备写文件
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

	////设置指定工具项的宽度并获取新的区域  20是宽度
	////index = m_wndToolBar.CommandToIndex(ID_NET_DEVS);
	//index = 3;
	//m_wndToolBar.SetButtonInfo(index, ID_PLUGINS, TBBS_SEPARATOR, 1);
	//m_wndToolBar.GetItemRect(index, &rect);
	//rect.right = rect.left + width * 4;
	//offset = rect.right;
	//rect.bottom += 100;
	////rect.top += 2;
	////rect.bottom -= 5;
	//// 创建并显示控件
	//if (!m_wndPlugins.Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_VSCROLL, rect,
	//	&m_wndToolBar, ID_PLUGINS))
	//{
	//	TRACE0("Failed to create combo-box\n");
	//	return;
	//}
	//m_wndPlugins.ShowWindow(SW_HIDE);

	//设置指定工具项的宽度并获取新的区域  1是宽度
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
	//加载网卡数据
	std::vector<NetCardInfo> devs;
	PcapAnalyzer::LoadNetDevs(devs);
	std::vector<NetCardInfo>::const_iterator iter = devs.begin();
	for (; iter != devs.end(); iter++)
	{
		m_wndDevs.InsertString(m_wndDevs.GetCount(), iter->description.c_str());
	}
	m_wndDevs.SetCurSel(0);


	//设置指定工具项的宽度并获取新的区域  20是宽度
	//index=m_wndToolBar.GetCount();
	index = 4;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_START, TBBS_SEPARATOR, 1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// 创建并显示控件
	if (!m_StartCapture.Create("", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW | WS_CLIPCHILDREN, rect,
		&m_wndToolBar, ID_CAPTURE_START))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// 设置按钮1图片
	
	m_StartCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-start.png"), SplicFullFilePath("Res\\Button\\x-capture-start-white.png"));
	m_StartCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//設置按鈕顯示半透明貼膜
	m_StartCapture.ShowWindow(SW_HIDE);
	m_StartCapture.SizeToContent();

	//设置指定工具项的宽度并获取新的区域  80是宽度
	//index = m_wndToolBar.CommandToIndex(ID_CAPTURE_STOP);
	index = 5;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_STOP, TBBS_SEPARATOR, 1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// 创建并显示控件
	if (!m_StopCapture.Create("", WS_CHILD | WS_VISIBLE, rect,
		&m_wndToolBar, ID_CAPTURE_STOP))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// 设置按钮1图片
	m_StopCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-stop.png"), SplicFullFilePath("Res\\Button\\x-capture-stop-white.png"));
	m_StopCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//設置按鈕顯示半透明貼膜
	m_StopCapture.ShowWindow(SW_HIDE);

	//设置指定工具项的宽度并获取新的区域  80是宽度
	//index = m_wndToolBar.CommandToIndex(ID_CAPTURE_RESTART);
	index = 6;
	m_wndToolBar.SetButtonInfo(index, ID_CAPTURE_RESTART, TBBS_SEPARATOR,1);
	m_wndToolBar.GetItemRect(index, &rect);
	rect.left = offset;
	rect.right = rect.left + width;
	offset = rect.right;
	rect.top += 2;
	rect.bottom -= 2;
	// 创建并显示控件
	if (!m_RestartCapture.Create("", WS_CHILD | WS_VISIBLE, rect,
		&m_wndToolBar, ID_CAPTURE_RESTART))
	{
		TRACE0("Failed to create combo-box\n");
		return;
	}
	// 设置按钮1图片
	m_RestartCapture.SetImagePng(SplicFullFilePath("Res\\Button\\x-capture-restart.png"), SplicFullFilePath("Res\\Button\\x-capture-restart-turn1.png"));
	m_RestartCapture.SetTransparentColor(RGB(74, 144, 226), 100, 155);//設置按鈕顯示半透明貼膜
	m_RestartCapture.ShowWindow(SW_HIDE);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// 创建类视图
	//CString strClassView;
	//bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	//ASSERT(bNameValid);
	//if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建“类视图”窗口\n");
	//	return FALSE; // 未能创建
	//}

	// 创建文件视图
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建“文件视图”窗口\n");
		return FALSE; // 未能创建
	}

	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	// 创建属性窗口
	//CString strPropertiesWnd;
	//bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	//ASSERT(bNameValid);
	//if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("未能创建“属性”窗口\n");
	//	return FALSE; // 未能创建
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

// CMainFrame 诊断

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


// CMainFrame 消息处理程序

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
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
	// 基类将执行真正的工作

	if (!CMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// 为所有用户工具栏启用自定义按钮
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
	//必须在主界面ui绘制结束才能进行其它UI绘制
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
	// TODO: 在此添加命令更新用户界面处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
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
	// TODO: 在此添加命令更新用户界面处理程序代码
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
