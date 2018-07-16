//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Sprite.cpp
// Description : The sprite class manages rendering and processing sprites in the game. It uses Windows GDI to load / use bitmaps for sprites.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "System.h"
#include "BackBuffer.h"

// This include
#include "Sprite.h"

// Static Variables
HDC CSprite::s_hSharedSpriteDC = 0;
int CSprite::s_iRefCount = 0;

// Static Function Prototypes

// Implementation
CSprite::CSprite()
{
	m_poPosition.x = 0;
	m_poPosition.y = 0;

	++s_iRefCount;
}

CSprite::~CSprite()
{
	DeleteObject(m_hSprite);
	DeleteObject(m_hMask);

	--s_iRefCount;

	if (s_iRefCount == 0)
	{
		DeleteDC(s_hSharedSpriteDC);
		s_hSharedSpriteDC = 0;
	}
}

bool CSprite::Initialise(int _iSpriteResourceID, int _iMaskResourceID)
{
	m_hInstance = CSystemClass::GetInstance()->GetAppInstance();
	m_pBackBuffer = CSystemClass::GetInstance()->GetBackBuffer();

	if (!s_hSharedSpriteDC)
	{
		s_hSharedSpriteDC = CreateCompatibleDC(NULL);
	}

	m_hSprite = LoadBitmap(m_hInstance, MAKEINTRESOURCE(_iSpriteResourceID));
	VALIDATE(m_hSprite);

	m_hMask = LoadBitmap(m_hInstance, MAKEINTRESOURCE(_iMaskResourceID));
	VALIDATE(m_hMask);

	GetObject(m_hSprite, sizeof(BITMAP), &m_bitmapSprite);
	GetObject(m_hMask, sizeof(BITMAP), &m_bitmapMask);

	return (true);
}

void CSprite::ChangeSprites(int _iSpriteResourceID, int _iMaskResourceID)
{
	DeleteObject(m_hSprite);
	DeleteObject(m_hMask);

	m_hSprite = LoadBitmap(m_hInstance, MAKEINTRESOURCE(_iSpriteResourceID));
	m_hMask = LoadBitmap(m_hInstance, MAKEINTRESOURCE(_iMaskResourceID));

	GetObject(m_hSprite, sizeof(BITMAP), &m_bitmapSprite);
	GetObject(m_hMask, sizeof(BITMAP), &m_bitmapMask);
}

void CSprite::Render()
{
	int iW = m_bitmapSprite.bmWidth;
	int iH = m_bitmapSprite.bmHeight;

	int iX = m_poPosition.x - (iW / 2);
	int iY = m_poPosition.y - (iH / 2);

	HGDIOBJ hOldObj = SelectObject(s_hSharedSpriteDC, m_hMask);

	BitBlt(m_pBackBuffer->GetBFDC(), iX, iY, iW, iH, s_hSharedSpriteDC, 0, 0, SRCAND);
	SelectObject(s_hSharedSpriteDC, m_hSprite);

	BitBlt(m_pBackBuffer->GetBFDC(), iX, iY, iW, iH, s_hSharedSpriteDC, 0, 0, SRCPAINT);
	SelectObject(s_hSharedSpriteDC, hOldObj);
}

POINT CSprite::GetPosition() const
{
	return(m_poPosition);
}

void CSprite::TranslateRelative(int _iX, int _iY)
{
	m_poPosition.x += _iX;
	m_poPosition.y += _iY;
}

void CSprite::TranslateAbsolute(int _iX, int _iY)
{
	m_poPosition.x = _iX;
	m_poPosition.y = _iY;
}