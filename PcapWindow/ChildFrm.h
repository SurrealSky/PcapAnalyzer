
// ChildFrm.h : CChildFrame ��Ľӿ�
//

#pragma once

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// ����
public:

// ����
public:

// ��д
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CSplitterWndEx  m_wndSplitter;
	CSplitterWndEx  m_wndSplitter2;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
