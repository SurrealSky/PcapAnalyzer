
// HexViewerView.h : CHexViewerView 类的接口
//

#pragma once
#include "afxwin.h"
#include"PcapWindowDoc.h"


class CHexViewerView : public CWnd
{
public:
	CHexViewerView();
	//DECLARE_DYNCREATE(CHexViewerView)

	// 特性
//public:
//	CPcapWindowDoc* GetDocument() const;

	// 操作
public:

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 实现
public:
	virtual ~CHexViewerView();

protected:

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
	CFont m_fontPrinter;
	int m_cxPrinter;
	int m_cxWidth;
	int m_cyPrinter;
	int m_cxOffset;
	int m_nLinesPerPage;
	int m_nLinesPerScroll;

public:
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void DrawSelText(CPcapWindowDoc* pDoc, CPaintDC* pDC);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	void PrintHeader(CPcapWindowDoc* pDoc, CDC* pDC, CPrintInfo* pInfo);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void ResetScroll();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCheckGoto();
public:
	// 文档的总行数
	UINT m_uLines;
	// 当前文件的大小
	ULONGLONG m_ullFileLength;
	// 文件内容
	BYTE* m_pFileData;
	// 获取特定行的内容
	void GetLineText(UINT uLine, CString & strText);
	afx_msg void OnPaint();
};

