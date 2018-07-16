//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Game.h
// Description : The game class is held by the system clas and controls all of the games aspects: Graphics, DX10, Menu system, client & server.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__GAME_H__)
#define __GAME_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "backbuffer.h"
#include "Mainmenu.h"
#include "Server.h"
#include "Client.h"
#include "play.h"

// Types

// Constants

// Prototypes

// Class
class CGame
{
	// Member Functions
	public:
		CGame();
		~CGame();
		bool Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight);
		void Render();
		void Process(float _fDeltaTick);

		CServer* CreateServer(tServerInfo _tServerInfo);
		CClient* CreateClient();

		void CloseConnections();

		void SetMouseState(bool _bMouseDown);

		CBackBuffer* GetBackBuffer() const;
		CMainMenu* GetMenu() const;
		CClient* GetClient() const;
		CPlay*	GetPlay() const;

		void SetState(GAMESTATE _eGameState);
		GAMESTATE GetState() const;

		void SetClientNull();

	protected:

	private:


	// Member Variables
	public:

	protected:	

	private:
		HWND			m_hWnd;	
		CBackBuffer*	m_pBackBuffer;

		// Menu Chain
		CMainMenu*		m_pMainMenu;

		// Game Chain
		CPlay*			m_pPlay;

		// Server and Clients (Networking)
		CServer*		m_pServer;
		CClient*		m_pClient;

		// State Machine for what state the game is in
		GAMESTATE		m_eGameState;
};

#endif // __GAME_H__