// DlgPlugins.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PcapWindow.h"
#include "DlgPlugins.h"
#include "afxdialogex.h"
#include"include.h"


// CDlgPlugins �Ի���

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


// CDlgPlugins ��Ϣ�������


BOOL CDlgPlugins::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowTextA("���");
	mPlugins.InsertString(0, "---------------------------");
	std::vector<std::string> plugins = PcapAnalyzer::GetAllPlugins();
	for (int i = 0; i < plugins.size(); i++)
	{
		mPlugins.InsertString(i + 1, plugins[i].c_str());
	}
	mPlugins.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgPlugins::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strPlugin;
	mPlugins.GetWindowText(strPlugin);
	plugin = strPlugin.GetString();
	if (plugin.size() == 0)
		plugin="";
	CDialogEx::OnOK();
}
