//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Backbuffer.cpp
// Description : Backbuffer is used to render to and then flipped with the screen (avoids flickering etc.)
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes

// This include
#include "BackBuffer.h"

// Static Variables

// Static Function Prototypes

// Implementation
CBackBuffer::CBackBuffer(): m_hWnd(0), m_hDC(0), m_hSurface(0), m_hOldObject(0), m_iWidth(0), m_iHeight(0)
{

}

CBackBuffer::~CBackBuffer()
{
	SelectObject(m_hDC, m_hOldObject);
	DeleteObject(m_hSurface);
	DeleteObject(m_hDC);
}

bool CBackBuffer::Initialise(HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hWnd = _hWnd;
	m_iWidth = _iWidth;
	m_iHeight = _iHeight;

	HDC hWindowDC = ::GetDC(m_hWnd);
	m_hDC = CreateCompatibleDC(hWindowDC);

	if(m_hDC == NULL)
	{
		return(false);
	}

	m_hSurface = CreateCompatibleBitmap(hWindowDC, m_iWidth, m_iHeight);

	if(m_hSurface == NULL)
	{
		return(false);
	}

	ReleaseDC(m_hWnd, hWindowDC);

	m_hOldObject = static_cast<HBITMAP>(SelectObject(m_hDC, m_hSurface));
	HBRUSH newBrush = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
	HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(m_hDC, newBrush));
	Rectangle(m_hDC, 0, 0, m_iWidth, m_iHeight);
	SelectObject(m_hDC, oldBrush);

	return (true);
}

void CBackBuffer::Clear()
{
	HBRUSH hOldBrush = static_cast<HBRUSH>(SelectObject(GetBFDC(), GetStockObject(BLACK_BRUSH)));
	Rectangle(GetBFDC(), 0, 0, GetWidth(), GetHeight());
	SelectObject(GetBFDC(), hOldBrush);
}

HDC CBackBuffer::GetBFDC() const
{
	return (m_hDC);
}

int CBackBuffer::GetWidth() const
{
	return (m_iWidth);
}

int CBackBuffer::GetHeight() const
{
	return (m_iHeight);
}

void CBackBuffer::Present()
{
	HDC hWndDC = ::GetDC(m_hWnd);
	BitBlt(hWndDC, 0, 0, m_iWidth, m_iHeight, m_hDC, 0, 0, SRCCOPY);
	ReleaseDC(m_hWnd, hWndDC);
}