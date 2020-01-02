
// HexViewerView.h : CHexViewerView ��Ľӿ�
//

#pragma once
#include "afxwin.h"
#include"PcapWindowDoc.h"


class CHexViewerView : public CWnd
{
public:
	CHexViewerView();
	//DECLARE_DYNCREATE(CHexViewerView)

	// ����
//public:
//	CPcapWindowDoc* GetDocument() const;

	// ����
public:

	// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// ʵ��
public:
	virtual ~CHexViewerView();

protected:

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	//��ͼ������ز���
	int m_cxChar;
	int m_cyChar;
	int m_nHSPos;
	int m_nVSPos;
	int m_nColSel;
	int m_nLineSel;
	CFont m_fontView;
	//��ӡ��ز���
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
	// �ĵ���������
	UINT m_uLines;
	// ��ǰ�ļ��Ĵ�С
	ULONGLONG m_ullFileLength;
	// �ļ�����
	BYTE* m_pFileData;
	// ��ȡ�ض��е�����
	void GetLineText(UINT uLine, CString & strText);
	afx_msg void OnPaint();
};

