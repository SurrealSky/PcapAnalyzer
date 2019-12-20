
// PcapWindowView.cpp : CPcapWindowView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PcapWindow.h"
#endif

#include "PcapWindowDoc.h"
#include "PcapWindowView.h"
#include"StreamsView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPcapWindowView

IMPLEMENT_DYNCREATE(CPcapWindowView, CView)

BEGIN_MESSAGE_MAP(CPcapWindowView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPcapWindowView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPcapWindowView ����/����

CPcapWindowView::CPcapWindowView()
{
	// TODO: �ڴ˴���ӹ������

}

CPcapWindowView::~CPcapWindowView()
{
}

BOOL CPcapWindowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPcapWindowView ����

void CPcapWindowView::OnDraw(CDC* /*pDC*/)
{
	CPcapWindowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPcapWindowView ��ӡ


void CPcapWindowView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPcapWindowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPcapWindowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPcapWindowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPcapWindowView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPcapWindowView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPcapWindowView ���

#ifdef _DEBUG
void CPcapWindowView::AssertValid() const
{
	CView::AssertValid();
}

void CPcapWindowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPcapWindowDoc* CPcapWindowView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPcapWindowDoc)));
	return (CPcapWindowDoc*)m_pDocument;
}
#endif //_DEBUG


// CPcapWindowView ��Ϣ�������
int CPcapWindowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

	return 0;
}


void CPcapWindowView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}
