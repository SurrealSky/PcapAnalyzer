#pragma once
#include "afxwin.h"
#include<string>


// CDlgPlugins �Ի���

class CDlgPlugins : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgPlugins)

public:
	CDlgPlugins(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPlugins();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox mPlugins;
	//CSkinComboBox mPlugins;
	std::string plugin;
public:
	afx_msg void OnBnClickedOk();
};
