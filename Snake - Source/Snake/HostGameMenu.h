//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : HostGameMenu.h
// Description : The host game menu controls setting up and creating a server
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__HOSTGAMEMENU_H__)
#define __HOSTGAMEMENU_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"
#include "Button.h"
#include "TextManager.h"

// Types
#define STARTBUTTON		0
#define BACKBUTTON		1

#define NAMEFIELD		0
#define ADDRESSFIELD	1
#define PORTFIELD		2

// Constants
class CGame;

class CHostGameMenu
{
	// Member Functions
	public:
		CHostGameMenu();
		~CHostGameMenu();

		bool Initialise(HWND _hWnd);
		void Render();	
		MENUSTATE Process();
		MENUSTATE ProcessClick(int _iOptionNumber);

		CTextManager* GetTextManager() const;

	protected:

	private:

	// Member Variables
	public:

	protected:

	private:
		HWND			m_hWnd;

		CSprite*		m_pBackground;
		CButton*		m_pButtons[2];
		CButton*		m_pFields[3];
		CTextManager*	m_pTextManager;
		CGame*			m_pGame;
		CClient*		m_pClient;

		bool			m_bHighlight[2];
		int				m_iHighlight[2];
	//	HFONT			m_hFont;
		HDC				m_hDC;
		
};

#endif // __HOSTGAMEMENU_H__