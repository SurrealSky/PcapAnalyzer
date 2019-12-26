// PngButton.cpp : implementation file
//
#include "stdafx.h"
#include "PngButton.h"
// CPngButton
IMPLEMENT_DYNAMIC(CPngButton, CButton)
CPngButton::CPngButton()
	: m_bFlagIsColorSet(FALSE)
	, m_bFlagIsTextSet(FALSE)
	, m_bFlagIsImageSet(FALSE)
	, m_bFlagIsFirst(TRUE)
	, m_bMouseOnButton(FALSE)
	, m_bIsPressed(FALSE)
	, m_bIsFocused(FALSE)
	//, m_iTitleSize(90)
	, m_iOnTransparent(0)
	, m_iDownTransparent(0)
	, m_iState(Disable)
	, m_bFlagContinueShow(FALSE)
{
	//DEL	initGDI();
	m_BaseColor = RGB(0, 0, 0);
	m_FontColor = RGB(0, 0, 0);
}
CPngButton::~CPngButton()
{
}

BEGIN_MESSAGE_MAP(CPngButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

// CPngButton message handlers
void CPngButton::LoadImage(CImage &image, UINT nID)
{
	HINSTANCE hInst = AfxGetResourceHandle();
	HRSRC hRsrc = ::FindResource(hInst, MAKEINTRESOURCE(nID), _T("PNG"));
	if (hRsrc == NULL)
		return;
	DWORD len = SizeofResource(hInst, hRsrc);
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
	if (lpRsrc == NULL)
		return;
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
	memcpy(pmem, lpRsrc, len);
	IStream* pstm;
	CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);
	image.Destroy();
	image.Load(pstm);
	GlobalUnlock(m_hMem);
	GlobalFree(m_hMem);
	pstm->Release();
	FreeResource(lpRsrc);
	if (image.GetBPP() == 32)
	{
		for (int i = 0; i < image.GetWidth(); i++)
		{
			for (int j = 0; j < image.GetHeight(); j++)
			{
				unsigned char* pucColor = reinterpret_cast<unsigned char *>(image.GetPixelAddress(i, j));
				pucColor[0] = pucColor[0] * pucColor[3] / 255;
				pucColor[1] = pucColor[1] * pucColor[3] / 255;
				pucColor[2] = pucColor[2] * pucColor[3] / 255;
			}
		}
	}
}
BOOL CPngButton::LoadImage(CImage &image, const CString& Path)
{
	CDC *pDC = GetDC();
	image.Destroy();
	image.Load(Path);
	if (image.IsNull())
	{
		return FALSE;
	}
	if (image.GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
	{
		int i;
		int j;
		for (i = 0; i < image.GetWidth(); i++)
		{
			for (j = 0; j < image.GetHeight(); j++)
			{
				byte *pByte = (byte *)image.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}
	ReleaseDC(pDC);
}
/*�z�y���Дఴ�o��B����Ϣ���܂�*/
void CPngButton::CancelHover(void)
{
	//����:ȡ����ˑҸ����Iλ
	//����:�o
	//����ֵ:�o
	if (m_bMouseOnButton)
	{
		m_bMouseOnButton = FALSE;
		Invalidate();
	} // if
} // End of CancelHover
void CPngButton::OnMouseMove(UINT nFlags, CPoint point)
{
	//����:����Ƅ���Ϣ
	//����:nFlag��ָʾ�������ⰴ���Ƿ���,point����ˮ�ǰλ��
	//����ֵ:�o
	CWnd*				wndUnderMouse = NULL;
	CWnd*				wndActive = this;
	TRACKMOUSEEVENT		csTME;
	CButton::OnMouseMove(nFlags, point);
	ClientToScreen(&point);
	wndUnderMouse = WindowFromPoint(point);
	// If the mouse enter the button with the left button pressed then do nothing
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE)//������I�����J�^�����^̎��
		return;
	if (wndUnderMouse && wndUnderMouse->m_hWnd == m_hWnd && wndActive)//�Дಢ���°��o��ǰ��B
	{
		if (!m_bMouseOnButton)
		{
			m_bMouseOnButton = TRUE;
			Invalidate();
			csTME.cbSize = sizeof(csTME);
			csTME.dwFlags = TME_LEAVE;
			csTME.hwndTrack = m_hWnd;
			::_TrackMouseEvent(&csTME);
		} // if
	}
	else CancelHover();
	CButton::OnMouseMove(nFlags, point);
}
LRESULT CPngButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//����:����x�_���o��Ϣ
	//����:wParam,lParam���λ�Å���
	//����ֵ:LRESULT
	CancelHover();//ȡ���Ҹ���B
	return 0;
} // End of OnMouseLeave
void CPngButton::OnKillFocus(CWnd* pNewWnd)
{
	//����:ʧȥ���c�r��Ϣ̎��
	//����:pNewWndָ��ָ��������뽹��Ĵ���
	//����ֵ:�o
	CButton::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here
	CancelHover();
}
BOOL CPngButton::OnEraseBkgnd(CDC* pDC)
{
	//����:����������Ϣ̎��
	//����:pDC��˰��o��DC
	//����ֵ:�o
	return TRUE;// CButton::OnEraseBkgnd(pDC);
}
/*�O�����P����*/
void CPngButton::PreSubclassWindow()
{
	//����:���o�O�����Lģʽ
	//����:�o
	//����ֵ:�o
	ModifyStyle(0, BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}
void CPngButton::SetSize(int iWidth, int iHeight)
{
	//����:�O�ð��o��С
	//����:iWidth�鰴�o����,iHeight�鰴�o�߶�
	//����ֵ:�o
	SetWindowPos(NULL, -1, -1, iWidth, iHeight,
		SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
}
BOOL CPngButton::SetTransparentColor(COLORREF color, UINT OnTransparent, UINT DownTransparent)
{
	//����:�O�ð�͸���NĤ����
	//����:color�NĤ���ɫ,OnTransparent����ˑҸ��ڰ��o�r��͸����,DownTransparent�ǰ��o���r��͸����
	//����ֵ:BOOL�Ƿ��O�óɹ�
	m_BaseColor = color;//�d���NĤ�ɫ
						//DEL	int ibrightness  = 0.299*GetRValue(color) + 0.587*GetGValue(color) + 0.114*GetBValue(color);//����
						//DEL	if (ibrightness > 225 || ibrightness < 15)
						//DEL	{
						//DEL		m_bFlagIsColorSet = FALSE;
						//DEL		return FALSE;
						//DEL	}
						//DEL	else
						//DEL	{
						//DEL		m_bFlagIsColorSet = TRUE;
						//DEL		return TRUE;
						//DEL	}
	if (OnTransparent < 0xFF && DownTransparent < 0xFF)//��͸��������ҪС�255,��t���O���NĤ
	{
		//�O���NĤ��Ч�����d���͸����
		m_bFlagIsColorSet = TRUE;
		m_iOnTransparent = OnTransparent;
		m_iDownTransparent = DownTransparent;
	}
	else {
		m_bFlagIsColorSet = FALSE;//�O���NĤ�oЧ
	}
	return m_bFlagIsColorSet;//�����NĤ�O�à�B
}
void CPngButton::SetFontColor(COLORREF color)
{
	m_FontColor = color;
}
BOOL CPngButton::SetTitle(const CString &csTitle)
{
	//����:�O�Ø��}
	//����:csTitle����}
	//����ֵ:BOOL���}�Ƿ��@ʾ
	this->SetWindowText(csTitle);
	m_bFlagIsTextSet = TRUE;
	//m_font.DeleteObject();
	//m_font.CreatePointFont(m_iTitleSize, _T("΢ܛ�ź�"));
	return m_bFlagIsTextSet;
}
void CPngButton::SetImagePng(UINT nIDNormal, UINT nIDDisable)
{
	//����:�O�ð��o����PNG�DƬ
	//����:nID�鰴�o��ͨ��B��PNG�N�D�YԴ,nIDDisable�鰴�oʧ�ܠ�B�µ�PNG�N�D�YԴ
	//����ֵ:�o
	m_bFlagIsImageSet = TRUE;;
	if (m_bFlagIsImageSet)
	{
		LoadImage(m_image[Disable], nIDDisable);
		LoadImage(m_image[Normal], nIDNormal);
		m_iState = MaskMode;
	}
	SizeToContent();
}
void CPngButton::SetImagePng(const CString& PathNormal, const CString& PathDisable)
{
	m_bFlagIsImageSet = TRUE;;
	if (m_bFlagIsImageSet)
	{
		LoadImage(m_image[Disable], PathDisable);
		LoadImage(m_image[Normal], PathNormal);
		m_iState = MaskMode;
	}
	SizeToContent();
}
void CPngButton::SetImagePng(UINT nIDNormal, UINT nIDOn, UINT nIDDown, UINT nIDDisable)
{
	//����:�O�ð��o����PNG�DƬ
	//����:nID�鰴�o��ͨ��B��PNG�N�D�YԴ,nIDOn����ˑҸ��ڰ��o�Ϡ�B��PNG�N�D�YԴ,nIDDown�鰴�o��� �B��PNG�N�D�YԴ,nIDDisable�鰴�oʧ�ܠ�B�µ�PNG�N�D�YԴ
	//����ֵ:�o
	m_bFlagIsImageSet = TRUE;
	if (m_bFlagIsImageSet)
	{
		if (m_image[Disable].IsNull() == FALSE)
			m_image[Disable].Destroy();
		LoadImage(m_image[Disable], nIDDisable);
		if (m_image[Normal].IsNull() == FALSE)
			m_image[Normal].Destroy();
		LoadImage(m_image[Normal], nIDNormal);
		if (m_image[On].IsNull() == FALSE)
			m_image[On].Destroy();
		LoadImage(m_image[On], nIDOn);
		if (m_image[Down].IsNull() == FALSE)
			m_image[Down].Destroy();
		LoadImage(m_image[Down], nIDDown);
		m_iState = PictureMode;
	}
	SizeToContent();
}
void CPngButton::SetImagePng(const CString& PathNormal, const CString& PathOn, const CString& PathDown, const CString& PathDisable)
{
	m_bFlagIsImageSet = TRUE;;
	if (m_bFlagIsImageSet)
	{
		if (m_image[Disable].IsNull() == FALSE)
			m_image[Disable].Destroy();
		LoadImage(m_image[Disable], PathDisable);
		if (m_image[Normal].IsNull() == FALSE)
			m_image[Normal].Destroy();
		LoadImage(m_image[Normal], PathNormal);
		if (m_image[On].IsNull() == FALSE)
			m_image[On].Destroy();
		LoadImage(m_image[On], PathOn);
		if (m_image[Down].IsNull() == FALSE)
			m_image[Down].Destroy();
		LoadImage(m_image[Down], PathDown);
		m_iState = PictureMode;
	}
	SizeToContent();
}
void CPngButton::SetContinueShow(BOOL ContinueShow)
{
	//����:�O���Ƿ�һֱ�@ʾ�鰴� �B
	//����:ContinueShow���Ƿ�Ҫһֱ�@ʾ�鰴� �B
	//����ֵ:�o
	m_bFlagContinueShow = ContinueShow;
}
void CPngButton::SizeToContent(void)
{
	//����:ʹ���o��С���d��ĈDƬ��Сһ��
	//����:�o
	//����ֵ:�o
	if (m_image[1] != NULL)
	{
		SetWindowPos(NULL, -1, -1, m_image[1].GetWidth(), m_image[1].GetHeight(),
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	} // if
} // End of SizeToContent
void CPngButton::SaveBG(void)
{
	//����:���游���ړ�ס�ı���
	//����:�o
	//����ֵ:�o
	CRect rc;
	this->GetWindowRect(&rc);
	CDC* pDC = GetDC();
	CBitmap memBitmap;
	//�õ������ھ���Լ��ؼ��ڸ����ڵ�λ��
	CDC* pParentDC = GetParent()->GetDC();
	CPoint pt(0, 0);
	MapWindowPoints(GetParent(), &pt, 1);
	if (m_MemDCBG == NULL)//����ѽ�m_MemDCBG�ѽ������˾Ͳ����ل�����,��t�´��{�Õr���e�`
		m_MemDCBG.CreateCompatibleDC(pDC);
	//����ס�ĸ����ڱ������ֱ�����m_MemDCBG��
	memBitmap.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	m_MemDCBG.SelectObject(&memBitmap);
	m_MemDCBG.BitBlt(0, 0, rc.Width(), rc.Height(), pParentDC, pt.x, pt.y, SRCCOPY);

	::ReleaseDC(GetParent()->m_hWnd, *pParentDC);
	::ReleaseDC(this->m_hWnd, *pDC);
}
BOOL CPngButton::SetShowState(BOOL NeedColorSet, BOOL NeedTextSet, BOOL NeedImageSet)
{
	//����:�O�ð��o�L��
	//����:NeedColorSet���Ƿ��@ʾ�NĤ,NeedTextSet���Ƿ��ӡ����,NeedImageSet���Ƿ��ӡPNG�DƬ
	//����ֵ:�o
	BOOL bFlagSuccessSet = TRUE;
	//�O���Ƿ��@ʾ�NĤ
	if (NeedColorSet)
	{
		if (m_BaseColor == NULL)
		{
			bFlagSuccessSet = FALSE;
		}
		else
			m_bFlagIsColorSet = NeedColorSet;
	}
	else {
		m_bFlagIsColorSet = NeedColorSet;
	}
	//�O���Ƿ��@ʾ����
	m_bFlagIsTextSet = NeedTextSet;
	//�O���Ƿ��@ʾpng�YԴ
	if (m_image[Disable] == NULL)
		;
	else
		m_bFlagIsImageSet = NeedImageSet;
	return bFlagSuccessSet;
}
/*�L�u���o*/
void CPngButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	//����:�L�u���o������BitButtonNL�
	//����:lpDrawItemStruct���o����
	//����ֵ:�o
	m_bIsFocused = (lpDrawItemStruct->itemState & ODS_FOCUS);//���o�Ƿ��н��c
	m_bIsPressed = (lpDrawItemStruct->itemState & ODS_SELECTED);//���o�Ƿ���
	if (m_bFlagIsFirst)//��һ���{�ñ��游���ړ�ס�ı���
	{
		SaveBG();
		m_bFlagIsFirst = FALSE;
		DrawItem(lpDrawItemStruct);
	}
	else {
		CRect rc;
		this->GetWindowRect(&rc);
		CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
		pDC->SetBkMode(TRANSPARENT);

		CDC MemDC;
		CBitmap bmp;
		//�����p�����õ�MemDC
		MemDC.CreateCompatibleDC(pDC);
		bmp.CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
		MemDC.SelectObject(&bmp);
		MemDC.BitBlt(0, 0, rc.Width(), rc.Height(), &m_MemDCBG, 0, 0, SRCCOPY);  //�������ړ�ס�ı���
		if (IsWindowEnabled())
		{
			switch (m_iState)
			{
			case MaskMode:
				if ((m_bFlagIsColorSet && m_bMouseOnButton) || m_bFlagContinueShow)
				{
					//����͸���NĤ
					CDC MemDCBK;
					CDC* pDCDrawBG;
					pDCDrawBG = GetDC();
					CBitmap bmpBK;
					//������͸���NĤ
					MemDCBK.CreateCompatibleDC(pDCDrawBG);
					bmpBK.CreateCompatibleBitmap(pDCDrawBG, rc.Width(), rc.Height());
					MemDCBK.SelectObject(&bmpBK);
					MemDCBK.FillSolidRect(0, 0, rc.Width(), rc.Height(), m_BaseColor);
					//��͸�������O��
					BLENDFUNCTION bf;
					bf.BlendOp = AC_SRC_OVER;
					bf.BlendFlags = 0;
					if (m_bIsPressed || m_bFlagContinueShow)
						bf.SourceConstantAlpha = m_iDownTransparent;
					else
						bf.SourceConstantAlpha = m_iOnTransparent;
					bf.AlphaFormat = 0;
					MemDC.AlphaBlend(0, 0, rc.Width(), rc.Height(), &MemDCBK, 0, 0, rc.Width(), rc.Height(), bf);//�NĤ
					::ReleaseDC(this->m_hWnd, *pDCDrawBG);
				}//end if ((m_bFlagIsColorSet && m_bMouseOnButton ) || m_bFlagContinueShow)
				if (m_bFlagIsImageSet)
				{
					m_image[Normal].Draw(MemDC.m_hDC, 0, 0);
				}//end if (m_bFlagIsImageSet)
				break;
			case PictureMode:
				if (m_bMouseOnButton || m_bFlagContinueShow)
				{
					if (m_bIsPressed || m_bFlagContinueShow)
						m_image[Down].Draw((&MemDC)->m_hDC, 0, 0);
					else
						m_image[On].Draw((&MemDC)->m_hDC, 0, 0);
				}
				else {//end if (m_bMouseOnButton || m_bFlagContinueShow)
					m_image[Normal].Draw((&MemDC)->m_hDC, 0, 0);
				}//end if (m_bMouseOnButton || m_bFlagContinueShow)...else
				break;
			}//end switch(m_iState)
		}
		else {//endif(IsWindowEnabled())
			if (m_image[Disable] != NULL)
				m_image[Disable].Draw((&MemDC)->m_hDC, 0, 0);
			else
				m_image[Normal].Draw((&MemDC)->m_hDC, 0, 0);
		}
		if (m_bFlagIsTextSet)
		{
			//������
			CString sTitle;
			GetWindowText(sTitle);
			//�O�������L��
			SetBkMode(MemDC, TRANSPARENT);
			SetTextColor(MemDC, m_FontColor);
			//�O�î��ڿؼ�����
			CRect captionRect = lpDrawItemStruct->rcItem;
			captionRect.top--;
			MemDC.SelectObject(GetFont());
			MemDC.DrawText((LPCTSTR)sTitle, &captionRect, DT_SINGLELINE | DT_VCENTER | DT_WORDBREAK | DT_CENTER);
		}//end if (m_bFlagIsTextSet)
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &MemDC, 0, 0, SRCCOPY);
	}//end if (m_bFlagIsFirst)..else
}