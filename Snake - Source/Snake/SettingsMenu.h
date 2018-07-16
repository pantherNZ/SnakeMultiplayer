#pragma once

#if !defined(__SETTINGS_H__)
#define __SETTINGS_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"

// Types

// Constants

// Prototypes

// Class definition
class CSettingsMenu
{
	// Member Functions
	public:
		CSettingsMenu();
		~CSettingsMenu();

		bool Initialise(HWND _hWnd);
		MENUSTATE Process();
		void Render();

	protected:

	private:

	// Member Variables
	public:

	protected:

	private:
		HWND		m_hWnd;
		CSprite*	m_pBackground;
};

#endif // __SETTINGS_H__