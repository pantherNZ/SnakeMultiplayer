//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : JoinGameMenu.h
// Description : The join game menu is a part of the menu system and controls joining servers on the local network
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__JOINGAMEMENU_H__)
#define __JOINGAMEMENU_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"
#include "Button.h"
#include "TextManager.h"

// Types

// Constants
class CClient;

class CJoinGameMenu
{
	// Member Functions
	public:
		CJoinGameMenu();
		~CJoinGameMenu();

		bool Initialise(HWND _hwnd);
		void Render();
		MENUSTATE Process();
		MENUSTATE ProcessClick(int _iOptionNumber);
		MENUSTATE ProcessHighlight();

		void ProcessServerFields();

		void SetClient(CClient* _pClient);
		CTextManager* GetTextManager() const;

	protected:

	private:

	// Member Variables
	public:

	protected:

	private:
		// Handle to the window
		HWND				m_hWnd;

		// Classes used
		CSprite*			m_pBackground;
		CButton*			m_pButtons[2];
		CButton*			m_pFields[MAX_SERVERS_DISPLAYED];
		//CButton*			m_pNameField;
		CTextManager*		m_pTextManager;
		CClient*			m_pClient;

		// Highlights + 
		bool				m_bHighlight[2];
		int					m_iHighlight[2];		
		int					m_iCurrentServersDisplayed;
		float				m_fInterval;

		std::vector<tServerInfo*>	m_vServerInfo;
};

#endif // __JOINGAMEMENU_H__