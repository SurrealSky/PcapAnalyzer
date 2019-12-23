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
	afx_msg LRESULT OnStreamviewAddpacket(WPARAM wParam, LPARAM lParam);
public:
	virtual void OnInitialUpdate();
public:
	void AddPacket2UI(CSyncPacket*, std::string strExp);
};


