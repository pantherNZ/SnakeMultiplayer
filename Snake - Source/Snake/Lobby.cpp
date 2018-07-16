//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Lobby.cpp
// Description : The lobby menu controls the clients and startup in the game. It is the main interface between the menu system and the actual game.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "system.h"
#include "Backbuffer.h"

// This include
#include "Lobby.h"

// Static Variables

// Static Function Prototypes

// Implementation
CLobbyMenu::CLobbyMenu() : m_pBackground(nullptr), m_pTextManager(nullptr), m_bHighlight(false), m_iHighlight(-1), m_pCross(nullptr), m_pTick(nullptr)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pButtons[i] = nullptr;
	}

	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		m_vClients[i] = nullptr;
	}

	m_poReadyStart.x = 480;
	m_poReadyStart.y = 230;

	m_poOffset.x = 400;
	m_poOffset.y = 50;
}

CLobbyMenu::~CLobbyMenu()
{
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pTextManager);

	for (int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pButtons[i]);
	}

	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		DeleteOBJ(m_vClients[i]);
	}

	m_vClients.clear();

	DeleteOBJ(m_pTick);
	DeleteOBJ(m_pCross);
}

bool CLobbyMenu::Initialise(HWND _hWnd)
{
	m_hWnd = _hWnd;

	m_hDC = CSystemClass::GetInstance()->GetBackBuffer()->GetBFDC();

	// Create our background sprite
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	// Initialise the background sprite
	if (!m_pBackground->Initialise(IDB_LOBBYBACK_SPRITE, IDB_BACKGROUND_MASK))
	{
		MessageBox(_hWnd, L"Failed to create the lobby background", L"ERROR", MB_OK);
		return false;
	}

	m_pBackground->TranslateAbsolute(512, 360);

	for (int i = 0; i < 2; ++i)
	{
		// Create our button
		m_pButtons[i] = new CButton;
		VALIDATE(m_pButtons[i]);

		// Initialise the button sprite
		if (!m_pButtons[i]->Initialise(m_hWnd, IDB_NOTREADY_SPRITE + (i * 10), IDB_READY_SPRITE + (i * 10), IDB_OPTION_MASK))
		{
			MessageBox(m_hWnd, L"Failed to create the options buttons", L"ERROR", MB_OK);
			return false;
		}
	}

	// 0 is the ready button
	m_pButtons[STARTBUTTON]->SetPosition(785, 650);
	m_pButtons[STARTBUTTON]->SetOffset(170, 25);

	// 1 is the disconnect button
	m_pButtons[BACKBUTTON]->SetPosition(200, 650);
	m_pButtons[BACKBUTTON]->SetOffset(150, 30);

	m_pTextManager = new CTextManager;
	VALIDATE(m_pTextManager);

	if (!m_pTextManager->Initialise(m_hWnd, m_hDC, MAX_CLIENTS))
	{
		MessageBox(m_hWnd, L"Failed to create the text manager", L"ERROR", MB_OK);
		return false;
	}

	for (int i = 0; i < MAX_CLIENTS; ++i)
	{
		m_pTextManager->SetupReadWrite(i, true, false, false, false); // Users can only enter letters for their name
		m_pTextManager->SetMessage("", i);

		if (i < (MAX_CLIENTS / 2))
		{
			RECT R = {150, 220 + (i * 50), 0, 0};
			m_pTextManager->InputScreen(R, i);
		}
		else
		{
			RECT R = {620, 220 + (i % 8) * 50, 0, 0};
			m_pTextManager->InputScreen(R, i);
		}
		
		m_pTextManager->SetTextColour(i, tColour(255, 255, 255, WHITE));
	}

	m_pTextManager->SetFontSize(25);

	m_pCross = new CSprite;
	VALIDATE(m_pCross);

	if (!m_pCross->Initialise(IDB_CROSS_SPRITE, IDB_OPTION_MASK))
	{
		MessageBox(m_hWnd, L"Unable to create cross sprite", L"ERROR", MB_OK);
		return false;
	}

	m_pTick = new CSprite;
	VALIDATE(m_pTick);

	if (!m_pTick->Initialise(IDB_TICK_SPRITE, IDB_OPTION_MASK))
	{
		MessageBox(m_hWnd, L"Unable to create tick sprite", L"ERROR", MB_OK);
		return false;
	}

	m_pTextManager->Select();

	return (true);
}

void CLobbyMenu::Render()
{
	// Render background
	m_pBackground->Render();

	for (int i = 0; i < 2; ++i)
	{
		// render buttons
		m_pButtons[i]->Render();
	}

	// render text
	m_pTextManager->Render();

	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		// find the correct position to render the ticks / crosses for each player
		int iPosX = 0;
		int iPosY = 0;

		if (i > 7)
		{
			iPosX = m_poReadyStart.x + m_poOffset.x;
		}
		else
		{
			iPosX = m_poReadyStart.x;
		}

		int iOffset = (i % 8) * m_poOffset.y;
		iPosY = m_poReadyStart.y + iOffset;

		if (m_vClients[i]->bReady)
		{
			m_pTick->TranslateAbsolute(iPosX, iPosY);
			m_pTick->Render();
		}
		else
		{
			m_pCross->TranslateAbsolute(iPosX, iPosY);
			m_pCross->Render();			
		}
	}

	m_pCross->TranslateAbsolute(0, 0);
}

MENUSTATE CLobbyMenu::Process()
{
	// Process the textmanager
	m_pTextManager->Process();

	for (UINT i = 0; i < MAX_CLIENTS; ++i)
	{
		m_pTextManager->SetMessage("", i);
	}

	// We need to receive data from server and display it
	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		m_pTextManager->SetMessage(m_vClients[i]->sPlayerName.GetString(), i);
	}

	MENUSTATE tempState = ProccessButtons();
	return(tempState);
}

MENUSTATE CLobbyMenu::ProccessButtons()
{
	MENUSTATE tempState = LOBBYMENU;
	
	for (int i = 0; i < 2; ++i)
	{
		bool bCurrentHighlight = (m_bHighlight && m_iHighlight == i);
		bool bCanHighlight = (!m_bHighlight && m_iHighlight != i);

		bool bAuto = (i == 1);

		if(m_pButtons[i]->Process(bCurrentHighlight, bCanHighlight, bAuto))
		{
			tempState = ProcessClick(i);
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

	return(tempState);
}

MENUSTATE CLobbyMenu::ProcessClick(int _iOptionNumber)
{
	CSystemClass::GetInstance()->SetMouseState(false);

	switch (_iOptionNumber)
	{
		// Clicked ready
		case(0):
		{
			// Toggle READY status
			if (m_pButtons[0]->CheckHighlight())
			{
				m_pButtons[0]->ManualHighlightOff();
			}
			else
			{
				m_pButtons[0]->ManualHighlightOn();
			}

			// Change our ready status

			CSystemClass::GetInstance()->GetGameInstance()->GetClient()->ToggleReady();

			// Update the server
			CSystemClass::GetInstance()->GetGameInstance()->GetClient()->SendLobbyInfo();
		}
		break;

		case(1):
		{
			// Go back to the main menu
			CSystemClass::GetInstance()->GetGameInstance()->CloseConnections();
			return(MAINMENU);
		}
		break;

		default: break;
	};

	return(LOBBYMENU);
}

CTextManager* CLobbyMenu::GetTextManager() const
{
	return (m_pTextManager);
}

void CLobbyMenu::AddToClientList(CStringA _sName, bool _bReady, tColour _colour)
{
	bool bAdd = true;

	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		if (_sName == m_vClients[i]->sPlayerName)
		{
			bAdd = false;

			// If crColour has changed, update it
			if(_colour.iR != m_vClients[i]->colour.iR || _colour.iG != m_vClients[i]->colour.iG || _colour.iB != m_vClients[i]->colour.iB)
			{
				m_vClients[i]->colour = _colour;
				m_pTextManager->SetTextColour(i, _colour);
			}

			// If bReady has changed, update it
			if(_bReady != m_vClients[i]->bReady)
			{
				m_vClients[i]->bReady = _bReady;
			}

			break;
		}
	}

	if (_sName == "@NAMENOTSET@") // Make sure we don't add it if the name isn't set yet
	{
		bAdd = false;
	}

	if (bAdd)
	{
		tLobbyClientInfo* pNewClient = new tLobbyClientInfo;
		pNewClient->bReady = _bReady;
		pNewClient->sPlayerName = _sName;
		pNewClient->colour = _colour;
		m_vClients.push_back(pNewClient);
		m_pTextManager->SetTextColour(m_vClients.size() - 1, _colour);

	}
}

void CLobbyMenu::RemoveFromClientList(CStringA _sName)
{
	int iDisconnectedClient = -1;

	for (UINT i = 0; i < m_vClients.size(); ++i)
	{
		if (_sName == m_vClients[i]->sPlayerName)
		{
			iDisconnectedClient = i;
		}
	} 

	DeleteOBJ(m_vClients[iDisconnectedClient]);
	m_vClients.erase(m_vClients.begin() + iDisconnectedClient);
	m_pTextManager->SetMessage("", iDisconnectedClient);
}