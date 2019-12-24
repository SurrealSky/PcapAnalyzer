// DlgPlugins.cpp : 实现文件
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "DlgPlugins.h"
#include "afxdialogex.h"
#include"include.h"


// CDlgPlugins 对话框

IMPLEMENT_DYNAMIC(CDlgPlugins, CDialogEx)

CDlgPlugins::CDlgPlugins(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{
	plugin = "";
}

CDlgPlugins::~CDlgPlugins()
{
}

void CDlgPlugins::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mPlugins);
}


BEGIN_MESSAGE_MAP(CDlgPlugins, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgPlugins::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPlugins 消息处理程序


BOOL CDlgPlugins::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowTextA("插件");
	mPlugins.InsertString(0, "---------------------------");
	std::vector<std::string> plugins = PcapAnalyzer::GetAllPlugins();
	for (int i = 0; i < plugins.size(); i++)
	{
		mPlugins.InsertString(i + 1, plugins[i].c_str());
	}
	mPlugins.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CDlgPlugins::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strPlugin;
	mPlugins.GetWindowText(strPlugin);
	plugin = strPlugin.GetString();
	if (plugin.size() == 0)
		plugin="";
	CDialogEx::OnOK();
}
