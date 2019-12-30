
// PcapWindowDoc.cpp : CPcapWindowDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PcapWindow.h"
#endif

#include "PcapWindowDoc.h"
#include"StreamsView.h"
#include"PacketsView.h"
#include"DlgPlugins.h"
#include"AnalysisView.h"
#include"MainFrm.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPcapWindowDoc

IMPLEMENT_DYNCREATE(CPcapWindowDoc, CDocument)

BEGIN_MESSAGE_MAP(CPcapWindowDoc, CDocument)
END_MESSAGE_MAP()


// CPcapWindowDoc 构造/析构

CPcapWindowDoc::CPcapWindowDoc()
{
	curStream = 0;
	curPacket = 0;
	mSessions.initcallhander(this, _stream_call_handler);
}

CPcapWindowDoc::~CPcapWindowDoc()
{
	mSessions.ClearStream();
}


// CPcapWindowDoc 序列化

void CPcapWindowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CPcapWindowDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
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

// 搜索处理程序的支持
void CPcapWindowDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
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

// CPcapWindowDoc 诊断

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


// CPcapWindowDoc 命令
void CPcapWindowDoc::_stream_call_handler(void* uParam1, void* uParam2, unsigned int code)
{
	CPcapWindowDoc *p = static_cast<CPcapWindowDoc*>(uParam1);
	switch (code)
	{
		case 0:
		{
			p->AddStream2StreamView((CSyncStream*)uParam2);
		}break;
		case 1:
		{
			p->AddPacket2StreamView((CSyncStream*)uParam2);
			//p->AddPacket2PacketView((CSyncStream*)uParam2);
		}break;
	}
}

void CPcapWindowDoc::AddStream2StreamView(CSyncStream *stream)
{
	POSITION pos = m_viewList.GetHeadPosition();
	while (pos != NULL)
	{
		CView * cView = (CView *)m_viewList.GetNext(pos);
		if (cView != NULL)
		{
			if (cView->IsKindOf(RUNTIME_CLASS(CStreamsView)))
			{
				PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDSTREAM, (WPARAM)stream, (LPARAM)0);
			}
		}
	}
}

void CPcapWindowDoc::AddPacket2StreamView(CSyncStream *stream)
{
	POSITION pos = m_viewList.GetHeadPosition();
	while (pos != NULL)
	{
		CView * cView = (CView *)m_viewList.GetNext(pos);
		if (cView != NULL)
		{
			if (cView->IsKindOf(RUNTIME_CLASS(CStreamsView)))
			{
				PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDPACKET, (WPARAM)stream, (LPARAM)0);
			}
		}
	}
}

void CPcapWindowDoc::AddPacket2PacketView(CSyncStream *stream)
{
	POSITION pos = m_viewList.GetHeadPosition();
	while (pos != NULL)
	{
		CView * cView = (CView *)m_viewList.GetNext(pos);
		if (cView != NULL)
		{
			if (cView->IsKindOf(RUNTIME_CLASS(CPacketsView)))
			{
				PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDPACKET, (WPARAM)stream, (LPARAM)0);
			}
		}
	}
}

void CPcapWindowDoc::Packet2HexView(CSyncPacket* packet)
{
	CWinAppEx *pApp = (CWinAppEx *)AfxGetApp();
	pApp->GetMainWnd()->SendMessage(WM_HEXVIEW_PACKET, (WPARAM)packet, (LPARAM)0);
}

void CPcapWindowDoc::Result2AnalysisView(std::map<std::string, std::string> result)
{
	curResult = result;
	POSITION pos = m_viewList.GetHeadPosition();
	while (pos != NULL)
	{
		CView * cView = (CView *)m_viewList.GetNext(pos);
		if (cView != NULL)
		{
			if (cView->IsKindOf(RUNTIME_CLASS(CAnalysisView)))
			{
				PostMessage(cView->m_hWnd, WM_STREAMVIEW_ADDPACKET, (WPARAM)0, (LPARAM)0);
			}
		}
	}
}

BOOL CPcapWindowDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	// TODO:  在此添加您专用的创建代码
	std::string plugin = "";
	CDlgPlugins mdlg;
	INT_PTR RESULT=mdlg.DoModal();
	if (RESULT == IDOK)
	{
		SetPathName(lpszPathName, TRUE);
		if (!CACap.OpenPcapFileByPacket(mSessions, GetPathName().GetString(), mdlg.plugin))
		{
			AfxMessageBox("PCAP文件错误!");
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CPcapWindowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (pMain)
	{
		CString strdev;
		pMain->m_wndDevs.GetWindowTextA(strdev);
		if (strdev.GetLength())
		{
			std::string plugin = "";
			CDlgPlugins mdlg;
			INT_PTR RESULT = mdlg.DoModal();
			if (RESULT == IDOK)
			{
				if (!CACap.StartOpenSniffer(mSessions, strdev.GetString(), mdlg.plugin))
				{
					AfxMessageBox("PCAP文件错误!");
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}

afx_msg void CPcapWindowDoc::OnCaptureStop()
{
	CACap.StopOpenSniffer();
}

afx_msg void CPcapWindowDoc::OnCaptureRestart()
{
	// CACap();
	if (CACap.IsSniffing())
	{
		CACap.StopOpenSniffer();
		CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
		if (pMain)
		{
			CString strdev;
			pMain->m_wndDevs.GetWindowTextA(strdev);
			if (strdev.GetLength())
			{
				std::string plugin = "";
				CDlgPlugins mdlg;
				INT_PTR RESULT = mdlg.DoModal();
				if (RESULT == IDOK)
				{
					if (!CACap.StartOpenSniffer(mSessions, strdev.GetString(), mdlg.plugin))
					{
						AfxMessageBox("PCAP文件错误!");
						return;
					}
				}
				else
				{
					return;
				}
			}
		}
	}
}

void CPcapWindowDoc::OnCloseDocument()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (CACap.IsSniffing())
		CACap.StopOpenSniffer();
	CDocument::OnCloseDocument();
}
