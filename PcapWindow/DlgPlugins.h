#pragma once
#include "afxwin.h"
#include<string>


// CDlgPlugins 对话框

class CDlgPlugins : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlugins)

public:
	CDlgPlugins(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPlugins();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox mPlugins;
	//CSkinComboBox mPlugins;
	std::string plugin;
public:
	afx_msg void OnBnClickedOk();
};
