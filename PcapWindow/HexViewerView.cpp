
// HexViewerView.cpp : CHexViewerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣

#include "PcapWindowDoc.h"
#include "HexViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHexViewerView

//IMPLEMENT_DYNCREATE(CHexViewerView, CView)

BEGIN_MESSAGE_MAP(CHexViewerView, CWnd)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETTINGCHANGE()
	ON_WM_SIZE()
	//ON_COMMAND(ID_CHECK_GOTO, &CHexViewerView::OnCheckGoto)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CHexViewerView ����/����

CHexViewerView::CHexViewerView()
	: m_cxChar(0)
	, m_cyChar(0)
	, m_nHSPos(0)
	, m_nVSPos(0)
	, m_nLineSel(-1)
	, m_nColSel(-1)
	, m_cxPrinter(0)
	, m_cxWidth(0)
	, m_cyPrinter(0)
	, m_cxOffset(0)
	, m_nLinesPerPage(0)
	, m_nLinesPerScroll(0)
{
	// TODO:  �ڴ˴���ӹ������
	m_ullFileLength = 200;
	m_pFileData = (BYTE*)malloc(m_ullFileLength);
	m_uLines = (m_ullFileLength + BYTES_ONE_LINE - 1) / BYTES_ONE_LINE;
}

CHexViewerView::~CHexViewerView()
{
}

BOOL CHexViewerView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= WS_HSCROLL | WS_VSCROLL;

	return CWnd::PreCreateWindow(cs);
}

int CHexViewerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//���������ʼ��
	m_fontView.CreatePointFont(100, TEXT("Courier New"));

	CClientDC dc(this);
	CFont *pOldFont = dc.SelectObject(&m_fontView);
	TEXTMETRIC tm;
	dc.GetTextMetrics(&tm);
	m_cxChar = tm.tmAveCharWidth;
	m_cyChar = tm.tmHeight + tm.tmExternalLeading;
	dc.SelectObject(pOldFont);

	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_nLinesPerScroll, 0);

	return 0;
}

void CHexViewerView::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CWnd::OnSettingChange(uFlags, lpszSection);

	::SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &m_nLinesPerScroll, 0);
}

void CHexViewerView::DrawSelText(CPcapWindowDoc* pDoc, CPaintDC* pDC)
{
	if (m_nColSel == -1 || m_nLineSel == -1)
		return;

	CString strLineText;
	GetLineText(m_nLineSel, strLineText);

	COLORREF OldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	COLORREF OldBkColor = pDC->SetBkColor(RGB(255, 0, 0));

	if (m_nColSel < 8) {
		pDC->TextOut((m_nColSel * 3 + 12)*m_cxChar, m_nLineSel*m_cyChar,
			strLineText.Mid(m_nColSel * 3 + 12, 2));
	}
	else {
		pDC->TextOut((m_nColSel * 3 + 14)*m_cxChar, m_nLineSel*m_cyChar,
			strLineText.Mid(m_nColSel * 3 + 14, 2));
	}
	pDC->TextOut((m_nColSel + 65)*m_cxChar, m_nLineSel*m_cyChar,
		strLineText.Mid(m_nColSel + 65, 1));

	pDC->SetTextColor(OldTextColor);
	pDC->SetBkColor(OldBkColor);
	return;
}

void CHexViewerView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	//��ӡ�и�ʽ
	CString strLine(TEXT("00000000    00 00 00 00 00 00 00 00 - 00 00 00 00 00 00 00 00    1234567890123456"));

	m_fontPrinter.CreatePointFont(100, TEXT("Courier New"), pDC);

	TEXTMETRIC tm;
	CFont *pOldFont = pDC->SelectObject(&m_fontPrinter);
	pDC->GetTextMetrics(&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	m_cxWidth = pDC->GetTextExtent(strLine, 81).cx;
	pDC->SelectObject(pOldFont);

	m_nLinesPerPage = (pDC->GetDeviceCaps(VERTRES) - (m_cyPrinter * 7)) / m_cyPrinter;
	UINT nMaxPage = max(1, (m_uLines +	(m_nLinesPerPage - 1)) / m_nLinesPerPage);
	pInfo->SetMaxPage(nMaxPage);

	m_cxOffset = (pDC->GetDeviceCaps(HORZRES) - m_cxWidth) / 2;
}

void CHexViewerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void CHexViewerView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	int nDocLines = m_uLines;
	if (!nDocLines)
		return;

	int nStart = (pInfo->m_nCurPage - 1) * m_nLinesPerPage;
	int nEnd = min(nDocLines, nStart + m_nLinesPerPage);
	CFont *pOldFont = pDC->SelectObject(&m_fontPrinter);
	PrintHeader(0, pDC, pInfo);
	for (int i = nStart; i < nEnd; ++i) {
		CString strLineText;
		GetLineText(i, strLineText);
		pDC->TextOut(m_cxOffset, m_cyPrinter * (5 + i - nStart), strLineText);
	}
	pDC->SelectObject(pOldFont);
}

void CHexViewerView::PrintHeader(CPcapWindowDoc* pDoc, CDC* pDC, CPrintInfo* pInfo)
{
	CString strFileName = pDoc->GetPathName();
	if (strFileName.GetLength() > 60)
		strFileName = pDoc->GetTitle();

	CString strPageNum;
	strPageNum.Format(TEXT("%d/%d"), pInfo->m_nCurPage, pInfo->GetMaxPage());

	pDC->TextOut(m_cxOffset, m_cyPrinter * 3, strFileName);
	UINT nOldTextAlign = pDC->SetTextAlign(TA_RIGHT);
	pDC->TextOut(m_cxOffset + m_cxWidth, m_cyPrinter * 3, strPageNum);
	pDC->SetTextAlign(nOldTextAlign);

	pDC->MoveTo(m_cxOffset, m_cyPrinter * 4);
	pDC->LineTo(m_cxOffset + m_cxWidth, m_cyPrinter * 4);
	pDC->MoveTo(m_cxOffset, m_cyPrinter*(m_nLinesPerPage + 6));
	pDC->LineTo(m_cxOffset + m_cxWidth, m_cyPrinter*(m_nLinesPerPage + 6));
}


void CHexViewerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CHexViewerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
//#ifndef SHARED_HANDLERS
//	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
//#endif
}


// CHexViewerView ��Ϣ�������


void CHexViewerView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	GetParent()->SendMessage(WM_COMMAND, ID_FILE_OPEN);

	CWnd::OnLButtonDblClk(nFlags, point);
}


void CHexViewerView::ResetScroll()
{
	SCROLLINFO si = { sizeof(si),SIF_PAGE | SIF_RANGE | SIF_POS };
	UINT &uLines = m_uLines;
	CRect rect;
	GetClientRect(&rect);
	if (uLines) {
		si.nMax = 80;
		si.nPage = min(81, rect.Width() / m_cxChar);
	}
	si.nPos = m_nHSPos;
	SetScrollInfo(SB_HORZ, &si);
	GetScrollInfo(SB_HORZ, &si);

	if (si.nPos != m_nHSPos) {
		ScrollWindow((m_nHSPos - si.nPos)*m_cxChar, 0);
		m_nHSPos = si.nPos;
	}

	si.nPos = m_nVSPos;
	si.nMax = max(0, uLines - 1);
	si.nPage = rect.Height() / m_cyChar;
	SetScrollInfo(SB_VERT, &si);
	GetScrollInfo(SB_VERT, &si);

	if (si.nPos != m_nVSPos) {
		ScrollWindow(0, (m_nVSPos - si.nPos)*m_cyChar);
		m_nVSPos = si.nPos;
	}
}


void CHexViewerView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si = { sizeof(si), SIF_ALL };
	GetScrollInfo(SB_HORZ, &si);

	switch (nSBCode) {
	case SB_LEFT:
		si.nPos = 0;
		break;
	case SB_RIGHT:
		si.nPos = si.nMax;
		break;
	case SB_LINELEFT:
		si.nPos -= 1;
		break;
	case SB_LINERIGHT:
		si.nPos += 1;
		break;
	case SB_PAGELEFT:
		si.nPos -= si.nPage;
		break;
	case SB_PAGERIGHT:
		si.nPos += si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	default:
		break;
	}
	si.fMask = SIF_POS;
	SetScrollInfo(SB_HORZ, &si);
	GetScrollInfo(SB_HORZ, &si);

	if (si.nPos != m_nHSPos) {
		ScrollWindow((m_nHSPos - si.nPos)*m_cxChar, 0);
		m_nHSPos = si.nPos;
	}

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CHexViewerView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO si = { sizeof(si), SIF_ALL };
	GetScrollInfo(SB_VERT, &si);

	switch (nSBCode) {
	case SB_TOP:
		si.nPos = 0;
		break;
	case SB_BOTTOM:
		si.nPos = si.nMax;
		break;
	case SB_LINELEFT:
		si.nPos -= 1;
		break;
	case SB_LINERIGHT:
		si.nPos += 1;
		break;
	case SB_PAGELEFT:
		si.nPos -= si.nPage;
		break;
	case SB_PAGERIGHT:
		si.nPos += si.nPage;
		break;
	case SB_THUMBTRACK:
		si.nPos = si.nTrackPos;
		break;
	default:
		break;
	}
	si.fMask = SIF_POS;
	SetScrollInfo(SB_VERT, &si);
	GetScrollInfo(SB_VERT, &si);

	if (si.nPos != m_nVSPos) {
		ScrollWindow(0, (m_nVSPos - si.nPos)*m_cyChar);
		m_nVSPos = si.nPos;
	}

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


BOOL CHexViewerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_nLinesPerScroll == WHEEL_PAGESCROLL) {
		SendMessage(WM_VSCROLL, (zDelta > 0 ? SB_PAGEUP : SB_PAGEDOWN));
	}
	else {
		int nLines = abs(zDelta) * m_nLinesPerScroll / WHEEL_DELTA;
		while (nLines--)
			SendMessage(WM_VSCROLL, (zDelta > 0 ? SB_LINEUP : SB_LINEDOWN));
	}

	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}


void CHexViewerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar) {
	case VK_HOME:
		OnVScroll(SB_TOP, 0, nullptr);
		break;
	case VK_END:
		OnVScroll(SB_BOTTOM, 0, nullptr);
		break;
	case VK_PRIOR:
		OnVScroll(SB_PAGEUP, 0, NULL);
		break;
	case VK_NEXT:
		OnVScroll(SB_PAGEDOWN, 0, NULL);
	default:
		break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CHexViewerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//����Ч������
	static int col[] = { -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, 0, 0, -1, 1, 1, -1, 2, 2, -1, 3, 3, -1,
		4, 4, -1, 5, 5, -1, 6, 6, -1, 7, 7, -1, -1, -1, 8, 8,
		-1, 9, 9, -1, 10, 10, -1, 11, 11, -1, 12, 12, -1,
		13, 13, -1, 14, 14, -1, 15, 15, -1, -1, -1, -1, 0,
		1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

	CPoint pt(point);
	CDC *pdc = GetDC();
	pdc->SetViewportOrg(-m_nHSPos * m_cxChar, -m_nVSPos * m_cyChar);
	pdc->DPtoLP(&pt);

	int nLines = m_uLines;
	//ȷ�����λ��������
	int nLine = pt.y / m_cyChar;
	if (nLine > nLines)
		nLine = -1;
	//���λ��������Ч��
	m_nColSel = pt.x / m_cxChar;
	if (m_nColSel > 80)
		m_nColSel = -1;
	else
		m_nColSel = col[m_nColSel];

	if (nLine >= 0 && nLine != m_nLineSel) {
		CString string;
		CFont *pOldFont = pdc->SelectObject(&m_fontView);
		GetLineText(m_nLineSel, string);
		pdc->TextOut(0, m_nLineSel*m_cyChar, string);
		pdc->SelectObject(pOldFont);
		m_nLineSel = nLine;
	}

	if (m_nColSel >= 0 && m_nLineSel >= 0) {
		CRect rect(0, m_nLineSel*m_cyChar, m_cxChar * 85, (m_nLineSel + 1)*m_cyChar);
		pdc->LPtoDP(&rect);
		InvalidateRect(&rect);
	}

	ReleaseDC(pdc);

	CWnd::OnLButtonDown(nFlags, point);
}


void CHexViewerView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	ResetScroll();
}



void CHexViewerView::OnCheckGoto()
{
	MessageBox(TEXT("asdf"));
}

// ��ȡ�ض��е�����
void CHexViewerView::GetLineText(UINT uLine, CString & strText)
{
	if (uLine >= m_uLines)
		return;

	BYTE p[BYTES_ONE_LINE + 1];
	::ZeroMemory(p, BYTES_ONE_LINE + 1);
	UINT uCount = BYTES_ONE_LINE,
		uIndex = uLine * BYTES_ONE_LINE;

	if ((uIndex + uCount) > m_ullFileLength)
		uCount = m_ullFileLength - uIndex;
	::CopyMemory(p, m_pFileData + uIndex, uCount);

	CStringA strTemp;
	strTemp.Format("%0.8X    %0.2X %0.2X %0.2X %0.2X "\
		"%0.2X %0.2X %0.2X %0.2X - %0.2X %0.2X %0.2X %0.2X "\
		"%0.2X %0.2X %0.2X %0.2X    ", uLine * 16, p[0], p[1],
		p[2], p[3], p[4], p[5], p[6], p[7], p[8], p[9], p[10],
		p[11], p[12], p[13], p[14], p[15]);

	for (UINT i = 0; i < uCount; ++i) {
		if (!IsCharAlphaNumericA(p[i]))
			p[i] = '.';
	}

	strTemp += CString(p);
	//�������Ҫ��ʾ���ַ����㣬����Ӧλ�õ��ַ���Ϊ�ո�
	if (uCount < 16) {
		UINT pos1 = 59;
		UINT pos2 = 60;
		UINT j = 16 - uCount;

		for (UINT i = 0; i<j; i++) {
			strTemp.SetAt(pos1, ' ');
			strTemp.SetAt(pos2, ' ');
			pos1 -= 3;
			pos2 -= 3;
			if (pos1 == 35) {
				strTemp.SetAt(35, ' ');
				strTemp.SetAt(36, ' ');
				pos1 = 33;
				pos2 = 34;
			}
		}
	}

	/*int nBytes = ::MultiByteToWideChar(CP_ACP, 0, (LPCCH)strTemp, -1, nullptr, 0);
	LPTSTR pBuff;
	try {
	pBuff = new TCHAR[nBytes];
	}
	catch (CMemoryException *e) {
	e->Delete();
	return;
	}
	::MultiByteToWideChar(CP_ACP, 0, strTemp, -1, pBuff, nBytes * sizeof(TCHAR));*/

	/*strText = pBuff;
	delete[] pBuff;*/
	strText = strTemp;
}

void CHexViewerView::OnPaint()
{
	CPaintDC pDC(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
					   //�����ļ�����ָ���жϵ�ǰ�Ƿ��ѳɹ����ļ�
	if (!m_pFileData) {
		//δ���ļ�����ʾ��ʾ��Ϣ
		CRect rect;
		GetClientRect(&rect);
		CFont font;
		font.CreatePointFont(200, TEXT("�����п�"));
		CFont *pOldFont = pDC.SelectObject(&font);
		COLORREF clrOld = pDC.SetTextColor(RGB(192, 192, 192));
		pDC.DrawText(TEXT("˫�����ڻ��ļ���ק�������Դ��ļ�"), rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC.SelectObject(pOldFont);
		pDC.SetTextColor(clrOld);
		return;
	}
	//���ݹ�������ǰλ�þ����ӿ�ԭ�㣬�ѷ����������
	pDC.SetViewportOrg(-m_nHSPos * m_cxChar, -m_nVSPos * m_cyChar);
	CRect rect;
	pDC.GetClipBox(&rect);//��ȡ��Ҫ�ػ�ľ���
	int nLineStart = max(0, rect.top / m_cyChar - 1);//����Ч���ξ�����Ҫ�ػ�Ŀ�ʼ��
	int nLineEnd = min(m_uLines, (UINT)rect.bottom / m_cyChar + 1);//������
																   //ȡ�ø�ʽ���ı��������������
	CString strLineText;
	CFont *pOldFont = pDC.SelectObject(&m_fontView);
	for (int i = nLineStart; i < nLineEnd; ++i) {
		GetLineText(i, strLineText);
		pDC.TextOut(0, i*m_cyChar, strLineText);
	}
	if (m_nLineSel >= nLineStart && m_nLineSel <= nLineEnd)
		DrawSelText(0, &pDC);
	pDC.SelectObject(pOldFont);
}
