
// HexViewerView.h : CHexViewerView 类的接口
//

#pragma once
#include "afxwin.h"


class CHexViewerView : public CWnd
{
public:
	CHexViewerView();
	virtual ~CHexViewerView();
	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	static const TCHAR m_szWndClassName[];
	
protected:
	static BOOL m_bIsRegistered;
	static BOOL RegisterWndClass();

	// 生成的消息映射函数
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	//视图窗口相关参数
	int m_cxChar;
	int m_cyChar;
	int m_nHSPos;
	int m_nVSPos;
	int m_nColSel;
	int m_nLineSel;
	CFont m_fontView;
	//打印相关参数
	//CFont m_fontPrinter;
	//int m_cxPrinter;
	//int m_cxWidth;
	//int m_cyPrinter;
	//int m_cxOffset;
	/*int m_nLinesPerPage;*/
	int m_nLinesPerScroll;

public:
	void DrawSelText(CPaintDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ResetScroll();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnEditSave();
public:
	// 文档的总行数
	UINT m_uLines;
	// 当前文件的大小
	ULONGLONG m_ullFileLength;
	// 文件内容
	BYTE* m_pFileData;
	// 获取特定行的内容
	void GetLineText(UINT uLine, CString & strText);
public:
	void SetDataView(BYTE *buffer, ULONGLONG size);
	void ClearView();
	static HMODULE GetCurrentModule(BOOL bRef/* = FALSE*/);
};

#define BYTES_ONE_LINE			16