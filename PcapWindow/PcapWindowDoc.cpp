
// PcapWindowDoc.cpp : CPcapWindowDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PcapWindow.h"
#endif

#include "PcapWindowDoc.h"

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
	CACap.InitSession(this, _stream_call_handler);
}

CPcapWindowDoc::~CPcapWindowDoc()
{
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
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
		//if (!CACap.OpenPcapFileByPacket(ar.m_strFileName.GetBuffer(0)))
		//{
		//	AfxMessageBox("PCAP�ļ�����!");
		//	return;
		//}
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
	CPcapWindowDoc *p = static_cast<CPcapWindowDoc*>(uParam1);
	p->UpdateAllViews(0, 0, p);
	//if (p->m_viewList.GetCount()>0)
	//{
	//	CView *cView = static_cast<CView *>(p->m_viewList.GetHead());
	//	p->UpdateAllViews(0, 0, p);
	//}

	CSyncStream *stream = static_cast<CSyncStream*>(uParam2);
	if (code == 0)
	{
		////�����
		//CString item = "";
		//item.Format("%d", count + 1);
		//p->mConnections.InsertItem(count, item);

		////ȡ��һ������ʱ���
		//p->mConnections.SetItemText(count, 1, p->CACap.FormatTime(stream->GetTime()).c_str());

		//CNetInfo net = stream->GetNetInfo();
		//char ip[0x30] = { 0 };
		//inet_ntop(AF_INET, (void*)&(net.srcip), ip, 16);
		//item.Format("%s", ip);
		//p->mConnections.SetItemText(count, 2, item);

		//inet_ntop(AF_INET, (void*)&(net.dstip), ip, 16);
		//item.Format("%s", ip);
		//p->mConnections.SetItemText(count, 3, item);

		//p->mConnections.SetItemText(count, 4, p->CACap.Protocol2String(net.proto).c_str());

		//item.Format("%d", stream->GetCount());
		//p->mConnections.SetItemText(count, 5, item);

		//p->mConnections.SetItemData(count, (DWORD_PTR)(stream));
	}
	else if (code == 1)
	{
		//������ݰ�
		//for (int i = 0; i < count; i++)
		//{
		//	if (((CSyncStream*)(p->mConnections.GetItemData(i)))->guid == stream->guid)
		//	{
		//		CString item = "";
		//		item.Format("%d", stream->GetCount());
		//		p->mConnections.SetItemText(i, 5, item);
		//		//�жϵ�ǰ���Ƿ�Ϊѡ����
		//		POSITION Position = p->mConnections.GetFirstSelectedItemPosition();
		//		int Item = p->mConnections.GetNextSelectedItem(Position);
		//		if (Item != -1)
		//		{
		//			if (((CSyncStream*)(p->mConnections.GetItemData(Item)))->guid == stream->guid)
		//			{
		//				if (p->mPackets.GetItemCount() > 0)
		//				{
		//					//����
		//					CString strExp;
		//					p->dfilterEdt.GetWindowTextA(strExp);
		//					p->AddPacket2UI(stream->GetBack(), strExp.GetBuffer(0));
		//				}
		//			}
		//		}
		//		break;
		//	}
		//}
	}
}
