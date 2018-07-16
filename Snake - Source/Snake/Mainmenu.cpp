//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Mainmenu.cpp
// Description : The main menu is a part of the game class and controls which menu is processed and rendered. Also manages the first screen (home menu).
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes

// This Include
#include "Mainmenu.h"
#include "resource.h"
#include "system.h"

// Implementation
CMainMenu::CMainMenu() : m_pBackground(nullptr), m_pHostGameMenu(nullptr), m_pJoinGameMenu(nullptr), m_bActive(true), m_bHighlight(false), m_iHighlight(-1),
						 m_eMenuState(MAINMENU), m_pLobby(nullptr)
{
	for (int i = 0; i < MENUBUTTONMAX; ++i)
	{
		m_pOptions[i] = nullptr;
	}
}

CMainMenu::~CMainMenu()
{
	DeleteOBJ(m_pHostGameMenu);
	DeleteOBJ(m_pJoinGameMenu);
	DeleteOBJ(m_pBackground);

	for (int i = 0; i < MENUBUTTONMAX; ++i)
	{
		DeleteOBJ(m_pOptions[i]);
	}

	DeleteOBJ(m_pLobby);
	CSystemClass::GetInstance()->SetLobbyMenuPtr(nullptr);
}

bool CMainMenu::Initialise(HWND _hWnd)
{
	m_bActive = true;
	m_hWnd = _hWnd;

	// Create our background sprite
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	// Initialise the background sprite
	if (!m_pBackground->Initialise(IDB_BACKGROUND_SPRITE, IDB_BACKGROUND_MASK))
	{
		MessageBox(m_hWnd, L"Failed to load the Main Menu background", L"ERROR", MB_OK);
		return false;
	}

	m_pBackground->TranslateAbsolute(512, 360);

	for (int i = 0; i < MENUBUTTONMAX; ++i)
	{
		// Create the join option
		m_pOptions[i] = new CButton;
		VALIDATE(m_pOptions[i]);

		// Initialise the join sprite
		if (!m_pOptions[i]->Initialise(m_hWnd, IDB_JOINSERVER_SPRITE + (10 * i),IDB_JOINSERVERHL_SPRITE + (10 * i), IDB_OPTION_MASK))
		{
			MessageBox(m_hWnd, L"Failed to load the Main Menu options", L"ERROR", MB_OK);
			return false;
		}
	}

	// Button positions & offsets 
	m_pOptions[JOINSERVER]->SetPosition(237, 500);
	m_pOptions[JOINSERVER]->SetOffset(170, 30);

	m_pOptions[HOSTSERVER]->SetPosition(240, 561);
	m_pOptions[HOSTSERVER]->SetOffset(180, 25);

	m_pOptions[QUIT]->SetPosition(124, 625); //(193, 545);//
	m_pOptions[QUIT]->SetOffset(65, 30);

	m_pOptions[HELP]->SetPosition(900, 630);
	m_pOptions[HELP]->SetOffset(75, 30);

	// Create the host game menu class
	m_pHostGameMenu = new CHostGameMenu;
	VALIDATE(m_pHostGameMenu);

	// Initialise the host game menu
	VALIDATE(m_pHostGameMenu->Initialise(_hWnd));

	// Create the join game menu class
	m_pJoinGameMenu = new CJoinGameMenu;
	VALIDATE(m_pJoinGameMenu);

	// Initialise the host game menu
	VALIDATE(m_pJoinGameMenu->Initialise(_hWnd));

	return true;
}

void CMainMenu::Render()
{
	switch(m_eMenuState)
	{
		case(MAINMENU):
		{
			m_pBackground->Render();
	
			for (int i = 0; i < MENUBUTTONMAX; ++i)
			{
				m_pOptions[i]->Render();
			}
		}
		break;

		case(JOINMENU):
		{
			m_pJoinGameMenu->Render();
		}
		break;

		case(HOSTMENU):
		{
			m_pHostGameMenu->Render();
		}
		break;

		case(LOBBYMENU):
		{
			m_pLobby->Render();
		}
		break;

		default: break;
	}
}

void CMainMenu::Process()
{
	switch(m_eMenuState)
	{
		case(MAINMENU):
		{
			// Process all the buttons
			ProcessMainMenu();
		}
		break;

		case(JOINMENU):
		{
			m_eMenuState = m_pJoinGameMenu->Process();
		}
		break;

		case(HOSTMENU):
		{
			m_eMenuState = m_pHostGameMenu->Process();
		}
		break;

		case(LOBBYMENU):
		{
			m_eMenuState = m_pLobby->Process();

			// If they have disconnected from the lobby (destroy the instance of the class)
			if(m_eMenuState == MAINMENU)
			{
				DeleteOBJ(m_pLobby);
				CSystemClass::GetInstance()->SetLobbyMenuPtr(nullptr);
			}
		}
		break;

		default: break;
	}
}

void CMainMenu::ProcessMainMenu()
{
	for (int i = 0; i < MENUBUTTONMAX; ++i)
	{
		bool bCurrentHighlight = (m_bHighlight && m_iHighlight == i);
		bool bCanHighlight = (!m_bHighlight && m_iHighlight != i);

		if(m_pOptions[i]->Process(bCurrentHighlight, bCanHighlight, true))
		{
			ProcessMouseClick(i);
		}

		if (bCurrentHighlight)
		{
			m_bHighlight = bCurrentHighlight;
			m_iHighlight = i;
		}
		else
		{
			if (m_iHighlight == i && !bCurrentHighlight)
			{
				m_bHighlight = false;
				m_iHighlight = -1;
			}
		}
	}
}

void CMainMenu::ProcessMouseClick(int _iOptionNumber)
{
	CSystemClass::GetInstance()->SetMouseState(false);

	switch(_iOptionNumber)
	{
		// Join Game Menu
		case(JOINSERVER):
		{
			m_eMenuState = JOINMENU;
			m_pJoinGameMenu->GetTextManager()->Select();

			// Create our client so it can ping for servers
			if (CSystemClass::GetInstance()->GetGameInstance()->GetClient() == nullptr)
			{
				m_pJoinGameMenu->SetClient(CSystemClass::GetInstance()->GetGameInstance()->CreateClient());		
			}
		}
		break;

		// Host Game Menu
		case(HOSTSERVER):
		{
			m_eMenuState = HOSTMENU;
			m_pHostGameMenu->GetTextManager()->Select();
			
			// If a client exists we need to delete it as the host server class will create its own client to manage
			if (CSystemClass::GetInstance()->GetGameInstance()->GetClient() != nullptr)
			{
				CSystemClass::GetInstance()->GetGameInstance()->SetClientNull();				
			}
		}
		break;

		// Quit
		case(QUIT):
		{
			if(MessageBox(m_hWnd, L"Are you sure?", L"Quit Game", MB_YESNO) == IDYES)
			{
				PostQuitMessage(0);
			}	
		}
		break;

		// Help
		case(HELP):
		{
			CStringW msg = L"SNAKE - GD1P03 - MDS - ALEX DENFORD - MICHAEL GREEN\n\n";
			msg.Append(L"How To Play: \nYou control a snake which moves around the screen. Your goal is to gather food and survive the longest.");
			msg.Append(L"Hitting other players or the walls will cause you to die. Score is increased over time as well as from eating food.\n\n");
			msg.Append(L"Controls: \nArrow Keys - Movement\nESC - Leave Current Game\n");
			MessageBox(m_hWnd, msg, L"Snake - Help", MB_OK);
		}
		break;

		default: break;
	}
}

bool CMainMenu::IsActive()
{
	return m_bActive;
}

CJoinGameMenu* CMainMenu::GetJoinMenu() const
{
	return (m_pJoinGameMenu);
}

CHostGameMenu* CMainMenu::GetHostMenu() const
{
	return (m_pHostGameMenu);
}

CLobbyMenu* CMainMenu::GetLobbyMenu() const
{
	return (m_pLobby);
}

void CMainMenu::CreateLobby()
{
	// Create the lobby class
	m_pLobby = new CLobbyMenu;

	// Initialise the lobby menu
	if(!m_pLobby->Initialise(m_hWnd))
	{
		PostQuitMessage(0);
	}

	CSystemClass::GetInstance()->SetLobbyMenuPtr(m_pLobby);
}