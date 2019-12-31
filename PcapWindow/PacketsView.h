#pragma once
#include"AutoComplete.h"



// CPacketsView ������ͼ

class CPacketsView : public CFormView
{
	DECLARE_DYNCREATE(CPacketsView)

protected:
	CPacketsView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CPacketsView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PACKETSVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl packets;
	CAutoComplete	dfilterEdt;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeCombo();
	afx_msg void OnCbnEditchangeCombo();
protected:
	afx_msg LRESULT OnUpdateui(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnPacketsNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
public:
	void AddPacket2UI(CSyncPacket*, std::string strExp);	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
};


