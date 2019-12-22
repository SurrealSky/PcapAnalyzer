
// PcapWindowDoc.cpp : CPcapWindowDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PcapWindow.h"
#endif

#include "PcapWindowDoc.h"
#include"StreamsView.h"
#include"PacketsView.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPcapWindowDoc

IMPLEMENT_DYNCREATE(CPcapWindowDoc, CDocument)

BEGIN_MESSAGE_MAP(CPcapWindowDoc, CDocument)
END_MESSAGE_MAP()


// CPcapWindowDoc ����/����

CPcapWindowDoc::CPcapWindowDoc()
{
	// TODO: �ڴ����һ���Թ������
	//CACap.InitSession(this, _stream_call_handler);
	mSessions.initcallhander(this, _stream_call_handler);
}

CPcapWindowDoc::~CPcapWindowDoc()
{
	mSessions.ClearStream();
}

BOOL CPcapWindowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CPcapWindowDoc ���л�

void CPcapWindowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
		if (!CACap.OpenPcapFileByPacket(mSessions,ar.m_strFileName.GetBuffer(0)))
		{
			AfxMessageBox("PCAP�ļ�����!");
			return;
		}
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CPcapWindowDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// ������������֧��
void CPcapWindowDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:     strSearchContent = _T("point;rectangle;circle;ole object;")��
	SetSearchContent(strSearchContent);
}

void CPcapWindowDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPcapWindowDoc ���

#ifdef _DEBUG
void CPcapWindowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPcapWindowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CPcapWindowDoc ����
void CPcapWindowDoc::_stream_call_handler(void* uParam1, void* uParam2, unsigned int code)
{
	Sleep(1000);
	return;
	CPcapWindowDoc *p = static_cast<CPcapWindowDoc*>(uParam1);
	POSITION pos = p->m_viewList.GetHeadPosition();
	while (pos != NULL)
	{
		CView * cView = (CView *)p->m_viewList.GetNext(pos);
		if (cView != NULL)
		{
			if (cView->IsKindOf(RUNTIME_CLASS(CStreamsView)))
			{
				switch (code)
				{
					case 0:
					{
						PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDSTREAM, (WPARAM)uParam2, (LPARAM)0);
					}break;
					case 1:
					{
						PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDPACKET, (WPARAM)uParam2, (LPARAM)0);
					}break;
				}
			}
			if (cView->IsKindOf(RUNTIME_CLASS(CPacketsView)))
			{
				if (code == 1)
				{
					PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDPACKET, (WPARAM)uParam2, (LPARAM)0);
				}
			}
		}
	}
}
