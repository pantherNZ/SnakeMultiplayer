//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Lobby.h
// Description : The lobby menu controls the clients and startup in the game. It is the main interface between the menu system and the actual game.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__LOBBY_H__)
#define __LOBBY_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"
#include "Button.h"
#include "TextManager.h"

// Types
#define READYBUTTON			0
#define DISCONNECTBUTTON	1

// Constants

class CLobbyMenu
{
	// Member Functions
	public:
		CLobbyMenu();
		~CLobbyMenu();

		bool Initialise(HWND _hWnd);
		void Render();	
		MENUSTATE Process();
		MENUSTATE ProcessClick(int _iOptionNumber);
		MENUSTATE ProccessButtons();

		void AddToClientList(CStringA _sName, bool _bReady, tColour _colour);
		void RemoveFromClientList(CStringA _sName);

		CTextManager* GetTextManager() const;

	protected:

	private:

	// Member Variables
	public:

	protected:

	private:
		HWND			m_hWnd;
		CSprite*		m_pBackground;
		CSprite*		m_pCross;
		CSprite*		m_pTick;
		CButton*		m_pButtons[2];
		//CButton*		m_pFields[16];
		bool			m_bHighlight;
		int				m_iHighlight;
		HDC				m_hDC;
		CTextManager*	m_pTextManager;
		POINT			m_poReadyStart;
		POINT			m_poOffset;

		std::vector<tLobbyClientInfo*>	m_vClients;
};

#endif // __LOBBY_H__