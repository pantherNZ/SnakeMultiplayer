//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Backbuffer.h
// Description : Backbuffer is used to render to and then flipped with the screen (avoids flickering etc.)
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__BACKBUFFER_H__)
#define __BACKBUFFER_H__

// Library Includes

// Local Includes
#include "utility.h"

// Types

// Constants

// Prototypes
class CBackBuffer
{
	// Member Functions
	public:
		CBackBuffer();
		~CBackBuffer();
		bool Initialise(HWND _hWnd, int _iWidth, int _iHeight);
		HDC GetBFDC() const;
		int GetHeight() const;
		int GetWidth() const;
		void Clear();
		void Present();

	protected:

	private:
		CBackBuffer(const CBackBuffer& _kr);
		CBackBuffer& operator= (const CBackBuffer& _kr);


	// Member Variables
	public:

	protected:
		HWND		m_hWnd;
		HDC			m_hDC;
		HBITMAP		m_hSurface;
		HBITMAP		m_hOldObject;
		int			m_iWidth;
		int			m_iHeight;

	private:

};

#endif // __BACKBUFFER_H__