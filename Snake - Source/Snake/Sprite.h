//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Sprite.h
// Description : The sprite class manages rendering and processing sprites in the game. It uses Windows GDI to load / use bitmaps for sprites.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__SPRITE_H__)
#define __SPRITE_H__

// Library Includes

// Local Includes
#include "utility.h"

// Types

// Constants

class CBackBuffer;

// Prototypes

// Class
class CSprite
{
	// Member Functions
	public:
		CSprite();
		~CSprite();
		bool Initialise(int _iResourceID, int _iMaskResourceID);
		void Render();
		void Process(float _fDeltaTick);
		POINT GetPosition() const;
		void TranslateRelative(int _iX, int _iY);
		void TranslateAbsolute(int _iX, int _iY);
		void ChangeSprites(int _iSpriteResourceID, int _iMaskResourceID);

	protected:
		 
	private:
		CSprite(const CSprite& _kr);
		CSprite& operator= (const CSprite& _kr);


	// Member Variables
	public:

	protected:
		//Center handle
		HINSTANCE		m_hInstance;
		POINT			m_poPosition;
		HBITMAP			m_hSprite;
		HBITMAP			m_hMask;
		BITMAP			m_bitmapSprite;
		BITMAP			m_bitmapMask;

		CBackBuffer*	m_pBackBuffer;

		static HDC		s_hSharedSpriteDC;
		static int		s_iRefCount;

	private:

};

#endif // __SPRITE_H__