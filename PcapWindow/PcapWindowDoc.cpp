
// PcapWindowDoc.cpp : CPcapWindowDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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


// CPcapWindowDoc 构造/析构

CPcapWindowDoc::CPcapWindowDoc()
{
	// TODO: 在此添加一次性构造代码
	CACap.InitSession(this, _stream_call_handler);
}

CPcapWindowDoc::~CPcapWindowDoc()
{
}

BOOL CPcapWindowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CPcapWindowDoc 序列化

void CPcapWindowDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
		//if (!CACap.OpenPcapFileByPacket(ar.m_strFileName.GetBuffer(0)))
		//{
		//	AfxMessageBox("PCAP文件错误!");
		//	return;
		//}
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
	p->UpdateAllViews(0, 0, p);
	//if (p->m_viewList.GetCount()>0)
	//{
	//	CView *cView = static_cast<CView *>(p->m_viewList.GetHead());
	//	p->UpdateAllViews(0, 0, p);
	//}

	CSyncStream *stream = static_cast<CSyncStream*>(uParam2);
	if (code == 0)
	{
		////添加流
		//CString item = "";
		//item.Format("%d", count + 1);
		//p->mConnections.InsertItem(count, item);

		////取第一个包的时间戳
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
		//添加数据包
		//for (int i = 0; i < count; i++)
		//{
		//	if (((CSyncStream*)(p->mConnections.GetItemData(i)))->guid == stream->guid)
		//	{
		//		CString item = "";
		//		item.Format("%d", stream->GetCount());
		//		p->mConnections.SetItemText(i, 5, item);
		//		//判断当前项是否为选中项
		//		POSITION Position = p->mConnections.GetFirstSelectedItemPosition();
		//		int Item = p->mConnections.GetNextSelectedItem(Position);
		//		if (Item != -1)
		//		{
		//			if (((CSyncStream*)(p->mConnections.GetItemData(Item)))->guid == stream->guid)
		//			{
		//				if (p->mPackets.GetItemCount() > 0)
		//				{
		//					//增加
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
