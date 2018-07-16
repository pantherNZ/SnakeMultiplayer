//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Mainmenu.h
// Description : The main menu is a part of the game class and controls which menu is processed and rendered. Also manages the first screen (home menu).
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__MAINMENU_H__)
#define __MAINMENU_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"
#include "JoinGameMenu.h"
#include "HostGameMenu.h"
#include "Lobby.h"

// Types

// Constants

// Prototypes
	 
// Class definition
class CMainMenu
{
	// Member Functions
	public:
		CMainMenu();
		~CMainMenu();

		bool Initialise(HWND m_hWnd);
		void Render();
		void Process();
		bool IsActive();

		CJoinGameMenu* GetJoinMenu() const;
		CHostGameMenu* GetHostMenu() const;
		CLobbyMenu* GetLobbyMenu() const;

		void CreateLobby();

	protected:

	private:
		void ProcessMouseClick(int _iOptionNumber);
		void ProcessMainMenu();

	// Member Variables
	public:

	protected:

	private:
		HWND			m_hWnd;
		bool			m_bActive, m_bHighlight;
		int				m_iHighlight;
		MENUSTATE		m_eMenuState;

		CSprite*		m_pBackground;
		CButton*		m_pOptions[4];
		CHostGameMenu*  m_pHostGameMenu;
		CJoinGameMenu*  m_pJoinGameMenu;
		CLobbyMenu*		m_pLobby;
};

#endif // __MAINMENU_H__