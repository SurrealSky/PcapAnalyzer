
// PcapAnalyzer.h : PcapAnalyzer Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CPcapAnalyzerApp:
// �йش����ʵ�֣������ PcapAnalyzer.cpp
//

class CPcapAnalyzerApp : public CWinAppEx
{
public:
	CPcapAnalyzerApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPcapAnalyzerApp theApp;
