//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Game.cpp
// Description : The game class is held by the system clas and controls all of the games aspects: Graphics, DX10, Menu system, client & server.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "utility.h"
#include "system.h"

// This Include
#include "Game.h"

// Implementation
CGame::CGame(): m_pBackBuffer(nullptr), m_pMainMenu(nullptr), m_pServer(nullptr), m_pClient(nullptr), m_eGameState(MENU), m_pPlay(nullptr)
{

}

CGame::~CGame()
{
	DeleteOBJ(m_pBackBuffer);
	DeleteOBJ(m_pMainMenu);
	DeleteOBJ(m_pServer);
	DeleteOBJ(m_pClient);
	DeleteOBJ(m_pPlay);
}

bool CGame::Initialise(HINSTANCE _hInstance, HWND _hWnd, int _iWidth, int _iHeight)
{
	m_hWnd = _hWnd;

	// Create backbuffer
	m_pBackBuffer = new CBackBuffer();
	if (!m_pBackBuffer)
	{
		MessageBox(m_hWnd, L"Failed to create Backbuffer class", L"ERROR", MB_OK);
		return (false);
	}

	// Initialise backbuffer
	VALIDATE(m_pBackBuffer->Initialise(_hWnd, _iWidth, _iHeight));

	// Create menu
	m_pMainMenu = new CMainMenu;
	if (!m_pMainMenu)
	{
		MessageBox(m_hWnd, L"Failed to create Menu class", L"ERROR", MB_OK);
		return (false);
	}

	// Initialise menu
	VALIDATE(m_pMainMenu->Initialise(_hWnd));

	return (true);
}

void CGame::Render()
{
	// Clear the backbuffer
	m_pBackBuffer->Clear();

	// Check what state the game is in
	switch (m_eGameState)
	{
		case (MENU):
		{
			m_pMainMenu->Render();
		}
		break;

		case (GAME):
		{
			// Render the game
			m_pPlay->Render();
		}
		break;

		default: break;
	};

	// Present the backbuffer to the screen
	m_pBackBuffer->Present();
}

void CGame::Process(float _fDeltaTick)
{
	switch (m_eGameState)
	{
		case (MENU):
		{
			// Process the main menu
			m_pMainMenu->Process();
		}
		break;

		case (GAME):
		{
			// Process the game
			m_pPlay->Process(_fDeltaTick);
		}
		break;
		
		default: break;
	};

	// If the server exists, process it
	if (m_pServer)
	{
		m_pServer->Process();
	}

	// Process the client
	if (m_pClient)
	{
		m_pClient->Process();
	}
}

CBackBuffer* CGame::GetBackBuffer() const
{
	return (m_pBackBuffer);
}

CMainMenu* CGame::GetMenu() const
{
	return (m_pMainMenu);
}

CServer* CGame::CreateServer(tServerInfo _tServerInfo)
{
	m_pServer = new CServer;
	m_pServer->CreateServer(_tServerInfo);

	return(m_pServer);
}

CClient* CGame::CreateClient()
{
	m_pClient = new CClient;
	m_pClient->CreateClient();

	return(m_pClient);
}

void CGame::CloseConnections()
{
	DeleteOBJ(m_pServer);
	DeleteOBJ(m_pClient);
}

CClient* CGame::GetClient() const
{
	return (m_pClient);
}

void CGame::SetState(GAMESTATE _eGameState)
{
	m_eGameState = _eGameState;

	if (_eGameState == GAME)
	{
		// Delete the menu system here
		DeleteOBJ(m_pMainMenu);

		m_pPlay = new CPlay;

		if (!m_pPlay->Initialise())
		{
			MessageBox(m_hWnd, L"Unable to load game screen", L"ERROR", MB_OK);
			PostQuitMessage(0);
		}

		// If this user is hosting the server
		if(m_pServer != nullptr)
		{
			// Setup the conencted clients game info
			m_pServer->GameStart();
		}
	}
	else if (_eGameState == MENU)
	{
		// Delete the menu system here
		DeleteOBJ(m_pPlay);
		DeleteOBJ(m_pClient);

		if(m_pServer != nullptr)
		{
			DeleteOBJ(m_pServer);
		}

		m_pMainMenu = new CMainMenu;

		if (!m_pMainMenu->Initialise(m_hWnd))
		{
			MessageBox(m_hWnd, L"Unable to load mainmenu screen", L"ERROR", MB_OK);
			PostQuitMessage(0);
		}

		//m_pClient->SetPlayPtr(nullptr);
		CSystemClass::GetInstance()->SetHostGamePtr(m_pMainMenu->GetHostMenu());
		CSystemClass::GetInstance()->SetJoinGamePtr(m_pMainMenu->GetJoinMenu());
	}
}

CPlay* CGame::GetPlay() const
{
	return (m_pPlay);
}

GAMESTATE CGame::GetState() const
{
	return (m_eGameState);
}

void CGame::SetClientNull()
{
	delete m_pClient;
	m_pClient = nullptr;
}