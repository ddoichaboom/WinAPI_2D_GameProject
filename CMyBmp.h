#pragma once

#include "Define.h"

class CMyBmp
{
public:
	CMyBmp();
	~CMyBmp();

public:
	HDC   Get_MemDC() const { return m_hMemDC; }
	int   Get_Width() const { return m_width; }
	int   Get_Height() const { return m_height; }

public:
	void  Load_Bmp(const TCHAR* pFilePath); 
	void	Release();

private:
	HDC			m_hMemDC	= nullptr;
	HBITMAP		m_hBitmap	= nullptr;
	HBITMAP		m_hOldBmp	= nullptr;
	int			m_width		= 0;
	int			m_height	= 0;

};

