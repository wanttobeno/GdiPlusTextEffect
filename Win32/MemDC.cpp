#include "MemDC.h"

CMemDC::CMemDC(HDC hdc,const RECT& rect)
{
	m_hdc = hdc;
	m_hMemDC = ::CreateCompatibleDC(hdc);
	m_rect = rect;
	m_hBitmap = ::CreateCompatibleBitmap(hdc,rect.right-rect.left,rect.bottom-rect.top);
	m_hOldBmp = (HBITMAP)::SelectObject(m_hMemDC,m_hBitmap);


	m_bRectPaint = false;
	memset(&m_RectPaint,0,sizeof(m_RectPaint));
}
// 析构的时候自动绘制
CMemDC::~CMemDC()
{
	if (m_bRectPaint)
	{
		::BitBlt(m_hdc,m_RectPaint.left,m_RectPaint.top,m_rect.right-m_rect.left,m_rect.bottom-m_rect.top,
			m_hMemDC,0,0,SRCCOPY);
	}
	else
	{
		::BitBlt(m_hdc,m_rect.left,m_rect.top,m_rect.right-m_rect.left,m_rect.bottom-m_rect.top,
			m_hMemDC,0,0,SRCCOPY);
	}
	::SelectObject(m_hMemDC,m_hOldBmp);
	::DeleteObject(m_hBitmap);
	::DeleteDC(m_hMemDC);
}


// 区域绘图
void  CMemDC::PaintRect(const RECT& paintRect,bool bRectPaint)
{
	m_bRectPaint = bRectPaint;
	if (bRectPaint)
	{
		memcpy(&m_RectPaint,&paintRect,sizeof(paintRect));
	}
}
