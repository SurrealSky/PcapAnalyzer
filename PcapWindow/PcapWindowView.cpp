
// PcapWindowView.cpp : CPcapWindowView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPcapWindowView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CPcapWindowView 构造/析构

CPcapWindowView::CPcapWindowView()
{
	// TODO: 在此处添加构造代码

}

CPcapWindowView::~CPcapWindowView()
{
}

BOOL CPcapWindowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPcapWindowView 绘制

void CPcapWindowView::OnDraw(CDC* /*pDC*/)
{
	CPcapWindowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CPcapWindowView 打印


void CPcapWindowView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPcapWindowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPcapWindowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPcapWindowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CPcapWindowView 诊断

#ifdef _DEBUG
void CPcapWindowView::AssertValid() const
{
	CView::AssertValid();
}

void CPcapWindowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPcapWindowDoc* CPcapWindowView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPcapWindowDoc)));
	return (CPcapWindowDoc*)m_pDocument;
}
#endif //_DEBUG


// CPcapWindowView 消息处理程序
int CPcapWindowView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;

	return 0;
}


void CPcapWindowView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
}
