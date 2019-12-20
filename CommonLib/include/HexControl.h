#pragma once
#include<Windows.h>

#define CUR_DEVICES_DPI96	96
#define CUR_DEVICES_DPI120	120

enum EditMode
{
	READONLY,	// readonly (no editing)
	INSERT,		// insertion
	OVERWRITE,	// overstrike (default)
	QUICKSAVE,	// quicksave
};

class HexControl
{
public:
	HexControl();
	~HexControl();
public:
	void CreateHexView(const HINSTANCE, const HWND);
	void SetEditMode(EditMode);
	void SetPosition(const int x, const int y, const int width, const int height, const int sysDPI= CUR_DEVICES_DPI96);
	void SetData(const BYTE *data, const unsigned __int64 len);
	unsigned __int64 GetDataLen();
	void GetData(BYTE *data, const unsigned __int64 len);
	void SetDataPtr(const DWORD_PTR ptr);
	DWORD_PTR GetDataPtr();
	void SetFont(const char *szFont, const int nSize, const bool bold);
	void SetFontColor(const BYTE r,const BYTE g,const BYTE b);
	void ClearFillColor();
	void SetBkFillColor(const __int64 offset,const __int64 length,const BYTE r, const BYTE g, const BYTE b);
private:
	HWND m_hWndParent;
	HWND m_hWndHexView;
	DWORD_PTR ptr;
};

