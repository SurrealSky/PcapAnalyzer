#pragma once
#include "afxwin.h"
// CPngButton

class CPngButton : public CButton
{
	//����:���d͸��PNG�O�ð�͸���İ��o�
	//ע:����F�NĤ�ɫ���ɫ��rՈ�z���Ƿ���SetColor���m_BaseColor��ʼ����RGB(0,0,0)
public:
	//-------------------------------------------------------------------
	//������SetColor
	//����:�O���|�����I�İ�͸���ɫ
	//����:COLORREF color - ���o�|����ı���ɫ
	//	   UINT OnTransparent = 0x0F - ��ˑҸ��rĤ��͸����
	//	   UINT DownTransparent = 0x3F - ���o���rĤ��͸����
	//����ֵ:�Д�ݔ��ֵ�Ƿ����
	//ע:�NĤ����,OnTransparent��DownTransparent��������0 - FF,ֵԽСԽ͸��
	//-------------------------------------------------------------------
	BOOL SetTransparentColor(COLORREF color, UINT OnTransparent = 0x0F, UINT DownTransparent = 0x3F);
	//�O�����w�ɫ
	void SetFontColor(COLORREF color);
	//-------------------------------------------------------------------
	//������:SetTitle
	//����:�O�ð��I�Ę��}
	//����:CString csTitle - ���I�@ʾ�ĈDƬ��Ϣ
	//����ֵ:�Д��Ƿ��O�óɹ�
	//-------------------------------------------------------------------
	BOOL SetTitle(const CString &csTitle);
	//-------------------------------------------------------------------
	//������SetImagePng
	//����:�O�ò���PNG�����YԴ
	//����:UINT nIDNormal - һ����r�����DƬ�YԴ��ID
	//	  :UINT nIDOn - ��ˑҸ���r�ĈDƬ�YԴ��ID
	//	  :UINT nIDDown - ���o������r�ĈDƬ�YԴ��ID
	//	  :UINT nIDDisable =0 - ���oʧ����r�ĈDƬ�YԴ��ID
	//����ֵ:�o
	//ע:SetImagePng(UINT nIDNormal,UINT nIDDisable =0);�Ԅ��O�Þ��NĤģʽ
	//	 SetImagePng(UINT nIDNormal,UINT nIDOn,UINT nIDDown,UINT nIDDisable =0);�Ԅ��O�Þ��N�Dģʽ
	//-------------------------------------------------------------------
	void SetImagePng(UINT nIDNormal, UINT nIDDisable = 0);
	void SetImagePng(const CString& PathNormal, const CString& PathDisable = _T(""));
	void SetImagePng(UINT nIDNormal, UINT nIDOn, UINT nIDDown, UINT nIDDisable = 0);
	void SetImagePng(const CString& PathNormal, const CString& PathOn, const CString& PathDown, const CString& PathDisable = _T(""));
	//-------------------------------------------------------------------
	//������SaveBG
	//����:���¿ؼ���ס�Ǳ���,��׃�����ڱ��������{��һ��
	//����:�o
	//����ֵ:�o
	//-------------------------------------------------------------------
	void SaveBG(void);
	//-------------------------------------------------------------------
	//������SizeToContent
	//����:ʹ���o��С��PNG�YԴ��Сһ��
	//����:�o
	//����ֵ:�o
	//-------------------------------------------------------------------
	void SizeToContent(void);
	//-------------------------------------------------------------------
	//������SetSize
	//����:�Զ��x�O�ÿؼ���С
	//����:int iWidth - �O�ÿؼ�����
	//    :int iHeight - �O�ÿؼ��߶�
	//����ֵ:�o
	//-------------------------------------------------------------------
	void SetSize(int iWidth, int iHeight);
	//-------------------------------------------------------------------
	//������SetShowState
	//����:�O�ð��o�L��
	//����:BOOL NeedColorSet - �O���Ƿ��@ʾ�NĤ
	//    :BOOL NeedTextSet - �O���Ƿ��ӡ����
	//	  :BOOL NeedImageSet - �O���Ƿ��ӡPNG�DƬ
	//����ֵ:�o
	//-------------------------------------------------------------------
	BOOL SetShowState(BOOL NeedColorSet, BOOL NeedTextSet, BOOL NeedImageSet);
	//-------------------------------------------------------------------
	//������SetContinueShow
	//����:�O�ð��o�Ƿ���Ҫһֱ�@ʾ�鰴� �B
	//����:BOOL ContinueShow - �Ƿ��@ʾ�鰴� �B
	//����ֵ:�o
	//-------------------------------------------------------------------
	void SetContinueShow(BOOL ContinueShow);
	//********************************************************************
	DECLARE_DYNAMIC(CPngButton)
protected:
	enum ButtonState
	{
		Disable = 0, Normal, On, Down
	};
	enum ButtonMode
	{
		MaskMode = 0, PictureMode
	};
public:
	CPngButton();
	virtual ~CPngButton();
protected:
	void LoadImage(CImage &image, UINT nID);
	BOOL LoadImage(CImage &image, const CString& Path);
	CImage m_image[4];
	void Draw(UINT nID, CDC* pDC, CRect rect);//��PNG�N�D
protected:
	DECLARE_MESSAGE_MAP()
	void CancelHover(void);//�޸���˲��ڑҸ��ڿؼ��ϵĘ��I
protected:
	CDC m_MemDCBG;//���ֵĸ����ړ�ס�ı���
	COLORREF m_BaseColor;//��͸���NĤ���ɫ
	COLORREF m_FontColor;//���w���ɫ
	BOOL m_bFlagIsFirst;//�Ƿ���һ��ȡ�ø����ڱ����Ę��Iλ
	BOOL m_bFlagIsColorSet;//�Ƿ���Ҫ�NĤ
	int m_iOnTransparent;//��ˑҸ��ڰ��o�ϕr��͸���NĤ��͸����
	int m_iDownTransparent;//��˰����ǰ�͸���NĤ��͸����
	BOOL m_bFlagIsTextSet;//�Ƿ���Ҫ��ӡ������
	BOOL m_bFlagIsImageSet;//�Ƿ���Ҫ��ӡ�DƬ
						   //CFont m_font;
						   //LOGFONT		m_lf;
						   //int m_iTitleSize;//���o���ֵĴ�С
	int m_iState;//���o���L��(0����ð�͸���NĤЧ��,1��ʹ��3������PNG�YԴ��^�֠�B)
protected:
	BOOL m_bMouseOnButton;//����Ƿ��ڰ��o��
	BOOL m_bIsPressed;//���I�Ƿ���
	BOOL m_bIsFocused;//���I�Ƿ�õ����c
	BOOL m_bFlagContinueShow;//�@����TRUE�r���o���L�D��һֱ�����ڰ�� �B
protected:
	virtual void PreSubclassWindow();//�O�ð��o�ָ�����L
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);//���L���o�D��
															 //һЩ�Дఴ�o��B��ϵ�y����
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);//��ֹ���ұ���
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);//����Ƅӵ�����r�Д��B
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);//����x�_�r��Ϣ�Д��B
	afx_msg void OnKillFocus(CWnd* pNewWnd);//ʧȥ���c�r�Д��B
};

