//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : JoinGameMenu.cpp
// Description : The join game menu is a part of the menu system and controls joining servers on the local network
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//
	
// Library Includes

// Local Includes
#include "resource.h"
#include "system.h"
#include "Mainmenu.h"
#include "Client.h"

// This include
#include "JoinGameMenu.h"

// Static Variables

// Static Function Prototypes

// Implementation
CJoinGameMenu::CJoinGameMenu() : m_pBackground(nullptr), m_pClient(nullptr), m_pTextManager(nullptr), m_iCurrentServersDisplayed(0), m_fInterval((float)clock())
{
	for (int i = 0; i < 2; ++i)
	{
		m_pButtons[i] = nullptr;
		m_bHighlight[i] = false;
		m_iHighlight[i] = -1;
	}

	for (int i = 0; i < MAX_SERVERS_DISPLAYED; ++i)
	{
		m_pFields[i] = nullptr;
	}
}

CJoinGameMenu::~CJoinGameMenu()
{
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pTextManager);

	m_pClient = nullptr;

	for (int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pButtons[i]);
	}

	for (int i = 0; i < MAX_SERVERS_DISPLAYED; ++i)
	{
		DeleteOBJ(m_pFields[i]);
	}

	for (UINT i = 0; i < m_vServerInfo.size(); ++i)
	{
		DeleteOBJ(m_vServerInfo[i]);
	}

	m_vServerInfo.clear();
}

bool CJoinGameMenu::Initialise(HWND _hWnd)
{
	m_hWnd = _hWnd;

	// Create our background sprite
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	// Initialise the background sprite
	if (!m_pBackground->Initialise(IDB_JOINBACK_SPRITE, IDB_BACKGROUND_MASK))
	{
		MessageBox(m_hWnd, L"Failed to create the Join Game Menu Background", L"ERROR", MB_OK);
		return false;
	}

	m_pBackground->TranslateAbsolute(512, 360);

	for (int i = 0; i < 2; ++i)
	{
		// Create our button
		m_pButtons[i] = new CButton;
		VALIDATE(m_pButtons[i]);
		
		// Initialise the button sprite
		if (!m_pButtons[i]->Initialise(m_hWnd, 10003 + (i * 100), 10004 + (i * 100), IDB_OPTION_MASK + i))
		{
			MessageBox(m_hWnd, L"Failed to create the options buttons", L"ERROR", MB_OK);
			return false;
		}
	}

	// 0 is the join server button
	m_pButtons[0]->SetPosition(650, 625);
	m_pButtons[0]->SetOffset(170, 30);

	// 1 is the back arrow
	m_pButtons[1]->SetPosition(90, 70);
	m_pButtons[1]->SetOffset(45, 28);

	m_pTextManager = new CTextManager;
	VALIDATE(m_pTextManager);

	if (!m_pTextManager->Initialise(m_hWnd, CSystemClass::GetInstance()->GetBackBuffer()->GetBFDC(), MAX_SERVERS_DISPLAYED + 1))
	{
		MessageBox(m_hWnd, L"Failed to create the text manager", L"ERROR", MB_OK);
		return false;
	}

	for (int i = 0; i < MAX_SERVERS_DISPLAYED; ++i)
	{
		m_pTextManager->SetupReadWrite(i, true, false, false, false);

		if (i == 0)
		{
			m_pTextManager->SetMessage("NO SERVERS FOUND...", i);
		}
		else
		{
			m_pTextManager->SetupReadWrite(i, true, false, false, false);
			m_pTextManager->SetMessage("", i);
		}
		RECT R = {260, 300 + (i * 50), 0, 0};
		m_pTextManager->InputScreen(R, i);
	}

	m_pTextManager->SetupReadWrite(MAX_SERVERS_DISPLAYED, true, false, true, false);
	m_pTextManager->SetMessage("", MAX_SERVERS_DISPLAYED);
	RECT R = {230, 230, 0, 0};
	m_pTextManager->InputScreen(R, MAX_SERVERS_DISPLAYED);
	m_pTextManager->SetMaxLength(MAX_SERVERS_DISPLAYED, 12);

	// Button 0 = name field
	m_pFields[0] = new CButton;
	VALIDATE(m_pFields[0]);

	// Intialise the fields
	if (!m_pFields[0]->Initialise(m_hWnd, IDB_NOTHING_SPRITE, IDB_FIELD_HIGHLIGHT, IDB_OPTION_MASK))
	{
		MessageBox(m_hWnd, L"Failed to create the name field button", L"ERROR", MB_OK);
		return false;
	}

	// name field pos & offset
	m_pFields[0]->SetPosition(522, 235);
	m_pFields[0]->SetOffset(320, 36);

	for (int i = 1; i < MAX_SERVERS_DISPLAYED; ++i)
	{
		m_pFields[i] = new CButton;
		VALIDATE(m_pFields[i]);

		// Intialise the fields
		if (!m_pFields[i]->Initialise(m_hWnd, IDB_NOTHING_SPRITE, IDB_SERVERHIGHLIGHT_SPRITE, IDB_OPTION_MASK))
		{
			MessageBox(m_hWnd, L"Failed to create the fields buttons", L"ERROR", MB_OK);
			return false;
		}

		m_pFields[i]->SetPosition(532, 256 + (i * 50));
		m_pFields[i]->SetOffset(285, 20);
	}

	return(true);
}

void CJoinGameMenu::Render()
{
	// Render the background sprite
	m_pBackground->Render();

	// Render the two clickable buttons (Back arrow & connect)
	for (int i = 0; i < 2; ++i)
	{
		m_pButtons[i]->Render();
	}
	
	// Render the five fields for servers
	for (int i = 0; i < MAX_SERVERS_DISPLAYED; ++i)
	{
		m_pFields[i]->Render();
	}

	// Render the text manager
	m_pTextManager->Render();
}

MENUSTATE CJoinGameMenu::Process()
{
	// Find the number of servers found
	// Process the client until : it finds all of the servers : it reaches a maximum number of attempts.
	int iServersFound = -1;
	int iMaxAttempts = 200;

	for (int i = 0; i < iMaxAttempts; ++i)
	{		
		m_pClient->Process();
		iServersFound = m_pClient->ServersFound();

		if (iServersFound == m_iCurrentServersDisplayed)
		{
			break;
		}
	}

	MENUSTATE tempState = ProcessHighlight();

	if(tempState != JOINMENU)
	{
		return(tempState);
	}

	// Check if the client has found a server
	if (m_iCurrentServersDisplayed != iServersFound)
	{
		// Clear the server vector so it can be 'refreshed'
		for (UINT i = 0; i < m_vServerInfo.size(); ++i)
		{
			DeleteOBJ(m_vServerInfo[i]);
		}

		m_vServerInfo.clear();

		// Set all the text man ager fields to empty to clear old server names
		for(int i = 0; i < MAX_SERVERS_DISPLAYED; ++i) 
		{			
			m_pTextManager->SetMessage( (i == 0 ? "No Servers Found..." : ""), i);
		}	

		if (iServersFound > 0)
		{
			for (int i = 0; i < MAX_SERVERS_DISPLAYED; ++i)
			{
				m_pTextManager->SetMessage("", i);
			}

			for (int i = 0; i < iServersFound; ++i)
			{
				// Add the found servers to the server vector.
				tServerInfo* tempInfo = m_pClient->GetServerInfo(i);		
				m_vServerInfo.push_back(tempInfo);
				m_pTextManager->SetMessage(m_vServerInfo[i]->sServerName.GetString(), i);
			}
		}

		if (m_iHighlight[1] > iServersFound)
		{
			m_pFields[m_iHighlight[1]]->ManualHighlightOff();

			m_bHighlight[1] = false;
			m_iHighlight[1] = -1;
		}
	}

	ProcessServerFields();

	m_iCurrentServersDisplayed = iServersFound;
	m_pTextManager->Process();

	// Only ping once every two seconds
	if(clock() >= m_fInterval + 2000.0f)
	{
		m_fInterval = (float)clock();

		// Ping for new servers
		m_pClient->Ping();
	}
	
	return(tempState);
}

MENUSTATE CJoinGameMenu::ProcessClick(int _iOptionNumber)
{
	CSystemClass::GetInstance()->SetMouseState(false);

	switch (_iOptionNumber)
	{
		case(0):
		{
			if(m_vServerInfo.size() > 0 && m_bHighlight[1])
			{
				// Then create lobby
				CSystemClass::GetInstance()->GetGameInstance()->GetMenu()->CreateLobby();

				// Get name from input field
				m_vServerInfo[m_iHighlight[1] - 1]->sPlayerName = m_pTextManager->GetString(MAX_SERVERS_DISPLAYED);

				if (m_pTextManager->GetString(MAX_SERVERS_DISPLAYED) == "")
				{
					MessageBox(m_hWnd, L"Please enter a name", L"ERROR", MB_OK);
					return (JOINMENU);
				}

				m_pClient->SetLobbyInfo(m_pTextManager->GetString(MAX_SERVERS_DISPLAYED).GetString(), false, tColour(255, 255, 255, WHITE));

				// Join the server	
				m_pClient->ConnectToServer(m_vServerInfo[m_iHighlight[1] - 1]);
				m_vServerInfo.erase(m_vServerInfo.begin());

				return(LOBBYMENU); 
			}
			
			return(JOINMENU);		
		}
		break;

		case(1):
		{
			// Go Back to the main menu
			return(MAINMENU);
		}
		break;

		default: break;
	};

	return(JOINMENU);
}

MENUSTATE CJoinGameMenu::ProcessHighlight()
{
	MENUSTATE tempstate = JOINMENU;

	for (int i = 0; i < 2; ++i)
	{
		bool bCurrentHighlight = (m_bHighlight[0] && m_iHighlight[0] == i);
		bool bCanHighlight = (!m_bHighlight[0] && m_iHighlight[0] != i);
		
		if(m_pButtons[i]->Process(bCurrentHighlight, bCanHighlight, true))
		{
			tempstate = ProcessClick(i);
		}

		if (bCurrentHighlight)
		{
			m_bHighlight[0] = bCurrentHighlight;
			m_iHighlight[0] = i;
		}
		else
		{
			if (m_iHighlight[0] == i && !bCurrentHighlight)
			{
				m_bHighlight[0] = false;
				m_iHighlight[0] = -1;
			}
		}
	}

	return (tempstate);
}

CTextManager* CJoinGameMenu::GetTextManager() const
{
	return (m_pTextManager);
}

void CJoinGameMenu::ProcessServerFields()
{
	// Process the fields
	for (int i = 0; i < m_iCurrentServersDisplayed + 1; i++)
	{
		bool bCurrentHighlight = (m_bHighlight[1] && m_iHighlight[1] == i);
		bool bCanHighlight = (!m_bHighlight[1] && m_iHighlight[1] != i);
	
		if (m_pFields[i]->Process(bCurrentHighlight, bCanHighlight, false))
		{
			// They clicked the field so highlight it and enable the ability to write to it.
			m_pFields[i]->ManualHighlightOn();
			m_bHighlight[1] = true;
			m_iHighlight[1] = i;
				 
			if (i == 0)
			{
				m_pTextManager->SetWriteAccess(MAX_SERVERS_DISPLAYED, true);
			}
			else
			{
				// User has clicked on server field. We just want to highlight it and set it as selected.
			}
		}
		else
		{
			// They didn't click the field
			if(CSystemClass::GetInstance()->GetMouseState())
			{
				m_pFields[i]->ManualHighlightOff();
				m_bHighlight[1] = false;
				m_iHighlight[1] = -1;	

				if (i == 0)
				{
					m_pTextManager->SetWriteAccess(MAX_SERVERS_DISPLAYED, false);
				}	
			}
		}
	}
}

void CJoinGameMenu::SetClient(CClient* _pClient)
{
	m_pClient = _pClient;
}