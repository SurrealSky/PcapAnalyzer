
// PcapAnalyzerView.cpp : CPcapAnalyzerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "PcapAnalyzer.h"
#endif

#include "PcapAnalyzerDoc.h"
#include "PcapAnalyzerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPcapAnalyzerView

IMPLEMENT_DYNCREATE(CPcapAnalyzerView, CView)

BEGIN_MESSAGE_MAP(CPcapAnalyzerView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPcapAnalyzerView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPcapAnalyzerView ����/����

CPcapAnalyzerView::CPcapAnalyzerView()
{
	// TODO: �ڴ˴���ӹ������

}

CPcapAnalyzerView::~CPcapAnalyzerView()
{
}

BOOL CPcapAnalyzerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CPcapAnalyzerView ����

void CPcapAnalyzerView::OnDraw(CDC* /*pDC*/)
{
	CPcapAnalyzerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CPcapAnalyzerView ��ӡ


void CPcapAnalyzerView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPcapAnalyzerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CPcapAnalyzerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CPcapAnalyzerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CPcapAnalyzerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPcapAnalyzerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPcapAnalyzerView ���

#ifdef _DEBUG
void CPcapAnalyzerView::AssertValid() const
{
	CView::AssertValid();
}

void CPcapAnalyzerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPcapAnalyzerDoc* CPcapAnalyzerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPcapAnalyzerDoc)));
	return (CPcapAnalyzerDoc*)m_pDocument;
}
#endif //_DEBUG


// CPcapAnalyzerView ��Ϣ�������
