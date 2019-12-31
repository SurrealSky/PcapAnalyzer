
// MainFrm.h : CMainFrame ��Ľӿ�
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include"include.h"
#include"PngButton.h"
#include<MFCUI\OwnerDrawCombox.h>


class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;
	CPngButton		m_StartCapture;
	CPngButton		m_StopCapture;
	CPngButton		m_RestartCapture;
public:
	CComboBox		m_wndDevs;
	//CComboBox		m_wndPlugins;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnCaptureStart();
	afx_msg void OnCaptureStop();
	afx_msg void OnCaptureRestart();
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	afx_msg LRESULT OnHexviewPacket(WPARAM wParam, LPARAM lParam);
private:
	void LoadUI();
	CString SplicFullFilePath(CString strExeModuleName);
public:
	afx_msg void OnUpdateCaptureStart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCaptureRestart(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCaptureStop(CCmdUI *pCmdUI);
protected:
	afx_msg LRESULT OnHexviewClear(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHexviewPair(WPARAM wParam, LPARAM lParam);
};


