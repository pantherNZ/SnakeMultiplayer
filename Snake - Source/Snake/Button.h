//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Button.h
// Description : Button class is used in the menu system to control highlighting, button clicks etc. (Holds a sprite)
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__BUTTON_H__)
#define __BUTTON_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "Sprite.h"

// Types

// Constants

// Prototypes
class CButton
{
	// Member Functions
	public:
		CButton();
		~CButton();

		bool Initialise(HWND _hWnd);
		bool Initialise(HWND _hWnd, int _iResourceID, int _iHighlightID, int _iMaskResourceID);

		POINT GetOffset() const;
		void SetOffset(int _iX, int _iY);

		POINT GetPosition() const;
		void SetPosition(int _iX, int _iY);

		// Check if the button is pressed. Return true if it is (pass in a reference to a bool if a highlight already exists)
		bool Process(bool& _rbIsHighlighted, bool _bAllowedToHighlight, bool _bAutoHighlight);	
		void Render();

		void ManualHighlightOn();
		void ManualHighlightOff();
		bool CheckHighlight();

	protected:

	private:


	// Member Variables
	public:

	protected:

	private:
		HWND		m_hWnd;
		CSprite*	m_pSprite;
		POINT		m_poPos;
		POINT		m_poOffset;

		int			m_iResourceID;
		int			m_iHighlightID;
		int			m_iMaskResourceID;

		bool		m_bHighlight;
};

#endif // __BUTTON_H__