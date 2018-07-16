//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : HostGameMenu.cpp
// Description : The host game menu controls setting up and creating a server
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "resource.h"
#include "system.h"
#include "Backbuffer.h"

// This include
#include "HostGameMenu.h"

// Static Variables

// Static Function Prototypes

// Implementation
CHostGameMenu::CHostGameMenu() : m_pBackground(nullptr), m_pTextManager(nullptr)
{
	for (int i = 0; i < 2; ++i)
	{
		m_pButtons[i] = nullptr;
		m_bHighlight[i] = false;
		m_iHighlight[i] = -1;
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pFields[i] = nullptr;
	}
}

CHostGameMenu::~CHostGameMenu()
{
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pTextManager);

	for (int i = 0; i < 2; ++i)
	{
		DeleteOBJ(m_pButtons[i]);
	}

	for (int i = 0; i < 3; ++i)
	{
		DeleteOBJ(m_pFields[i]);
	}
}

bool CHostGameMenu::Initialise(HWND _hWnd)
{
	m_hWnd = _hWnd;

	m_hDC = CSystemClass::GetInstance()->GetBackBuffer()->GetBFDC();

	// Create our background sprite
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	// Initialise the background sprite
	if (!m_pBackground->Initialise(IDB_HOSTBACK_SPRITE, IDB_BACKGROUND_MASK))
	{
		MessageBox(_hWnd, L"Failed to create the Host Game Menu Background", L"ERROR", MB_OK);
		return false;
	}

	m_pBackground->TranslateAbsolute(512, 360);

	for (int i = 0; i < 2; ++i)
	{
		// Create our button
		m_pButtons[i] = new CButton;
		VALIDATE(m_pButtons[i]);

		// Initialise the button sprite
		if (!m_pButtons[i]->Initialise(m_hWnd, 10102 + i, (10204 - (i * 100)), IDB_OPTION_MASK + i))
		{
			MessageBox(m_hWnd, L"Failed to create the options buttons", L"ERROR", MB_OK);
			return false;
		}
	}

	// 0 is the start button
	m_pButtons[STARTBUTTON]->SetPosition(740, 610);
	m_pButtons[STARTBUTTON]->SetOffset(110, 30);

	// 1 is the back arrow
	m_pButtons[BACKBUTTON]->SetPosition(90, 70);
	m_pButtons[BACKBUTTON]->SetOffset(45, 28);

	for (int i = 0; i < 3; ++i)
	{
		// Create the invisible buttons for the fields
		m_pFields[i] = new CButton;
		VALIDATE(m_pFields[i]);

		// Intialise the fields
		if (!m_pFields[i]->Initialise(m_hWnd, IDB_NOTHING_SPRITE, IDB_FIELD_HIGHLIGHT, IDB_OPTION_MASK))
		{
			MessageBox(m_hWnd, L"Failed to create the fields buttons", L"ERROR", MB_OK);
			return false;
		}
	}

	// 0 is the server name field
	m_pFields[NAMEFIELD]->SetPosition(510, 250);
	m_pFields[NAMEFIELD]->SetOffset(320, 36);

	// 1 is the player name field
	m_pFields[ADDRESSFIELD]->SetPosition(510, 386);
	m_pFields[ADDRESSFIELD]->SetOffset(320, 36);

	// 2 is the port field
	m_pFields[PORTFIELD]->SetPosition(510, 522);
	m_pFields[PORTFIELD]->SetOffset(320, 36);

	m_pTextManager = new CTextManager;
	VALIDATE(m_pTextManager);

	if (!m_pTextManager->Initialise(m_hWnd, m_hDC, 3))
	{
		MessageBox(m_hWnd, L"Failed to create the text manager", L"ERROR", MB_OK);
		return false;
	}

	m_pTextManager->SetMessage("", 2);
	m_pTextManager->SetupReadWrite(0, true, false, true, false); // Users can only enter letters for their name
	RECT R = {210, 240, 0, 0};
	m_pTextManager->InputScreen(R, 0);
	
	m_pTextManager->SetMessage("", 1);
	m_pTextManager->SetupReadWrite(1, true, false, true, false);
	RECT R2 = {210, 380, 0, 0};
	m_pTextManager->InputScreen(R2, 1);
	m_pTextManager->SetMaxLength(1, 12);
	
	m_pTextManager->SetMessage("60100", 2); // Enter a port between 60100 and 60200
	m_pTextManager->SetupReadWrite(2, true, false, false, true); // Users can only enter numbers for the port number
	RECT R3 = {210, 520, 0, 0};
	m_pTextManager->InputScreen(R3, 2);
	m_pTextManager->SetMaxLength(2, 5);

	// store client
	m_pGame = CSystemClass::GetInstance()->GetGameInstance();

	return(true);
}

void CHostGameMenu::Render()
{
	m_pBackground->Render();

	for (int i = 0; i < 2; ++i)
	{
		m_pButtons[i]->Render();
	}

	for (int i = 0; i < 3; ++i)
	{
		m_pFields[i]->Render();
	}

	m_pTextManager->Render();
}

MENUSTATE CHostGameMenu::Process()
{
	m_pTextManager->Process();

	MENUSTATE tempState = HOSTMENU;
	
	for (int i = 0; i < 2; ++i)
	{
		bool bCurrentHighlight = (m_bHighlight[0] && m_iHighlight[0] == i);
		bool bCanHighlight = (!m_bHighlight[0] && m_iHighlight[0] != i);

		if(m_pButtons[i]->Process(bCurrentHighlight, bCanHighlight, true))
		{
			tempState = ProcessClick(i);
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

	for (int i = 0; i < 3; i ++)
	{
		bool bCurrentHighlight = (m_bHighlight[1] && m_iHighlight[1] == i);
		bool bCanHighlight = (!m_bHighlight[1] && m_iHighlight[1] != i);

		if (m_pFields[i]->Process(bCurrentHighlight, bCanHighlight, false))
		{
			// They clicked the field so highlight it and enable the ability to write to it.
			m_pFields[i]->ManualHighlightOn();
			m_bHighlight[1] = true;
			m_iHighlight[1] = i;

			m_pTextManager->SetWriteAccess(i, true);
		}
		else
		{
			// They didn't click the field
			if(CSystemClass::GetInstance()->GetMouseState())
			{
				m_pFields[i]->ManualHighlightOff();
				m_bHighlight[1] = false;
				m_iHighlight[1] = -1;

				m_pTextManager->SetWriteAccess(i, false);
			}
		}
	}

	return(tempState);
}

MENUSTATE CHostGameMenu::ProcessClick(int _iOptionNumber)
{
	CSystemClass::GetInstance()->SetMouseState(false);

	switch (_iOptionNumber)
	{
		case(0):
		{
			// First check the name of the server and the port number are valid
			tServerInfo tSInfo;
			tSInfo.sServerName = m_pTextManager->GetString(0);
			tSInfo.sServerName.Append("\0");
			tSInfo.sPlayerName = m_pTextManager->GetString(1);
			tSInfo.sPlayerName.Append("\0");
			tSInfo.sPort = _wtoi(m_pTextManager->GetString(2));

			if (tSInfo.sServerName == "")
			{
				MessageBox(m_hWnd, L"Please enter a server name", L"ERROR", MB_OK);
				break;
			}

			if (tSInfo.sPlayerName == "")
			{
				MessageBox(m_hWnd, L"Please enter a player name", L"ERROR", MB_OK);
				break;
			}

			if (m_pTextManager->GetString(2) == "")
			{
				MessageBox(m_hWnd, L"Please enter a valid port number", L"ERROR", MB_OK);
				break;
			}

			if (tSInfo.sPort < 60100 || tSInfo.sPort > 60200)
			{
				MessageBox(m_hWnd, L"Please enter a valid port number", L"ERROR", MB_OK);
				break;
			}

			// Then start the server
			m_pGame->CreateServer(tSInfo);

			// Then create lobby
			m_pGame->GetMenu()->CreateLobby();

			// We also create a client
			m_pClient = m_pGame->CreateClient();
			m_pClient->SetLobbyInfo(tSInfo.sPlayerName, false, tColour(255, 255, 255, WHITE));

			bool bFound = false;
			int iCounter = 0;
			int iServer = -1;

			// We need to give our client some time to recieve the pong and add the server to its internal list
			while (!bFound && (iCounter < 5000))
			{
				m_pClient->Process(); 

				for (int i = 0; i < m_pClient->ServersFound(); ++i)
				{
					tServerInfo* pTempInfo = m_pClient->GetServerInfo(i);

					if (pTempInfo->sServerName == tSInfo.sServerName && pTempInfo->sPort == tSInfo.sPort)
					{
						iServer = i;
					}
				}

				bFound = (iServer != -1);
				iCounter++;
				Sleep(1);
			}

			if( !bFound )
			{
				MessageBox(NULL, L"UNABLE TO FIND LOCAL SERVER", L"ERROR", MB_OK);
				break;
			}

			// Join our new server 
			m_pClient->ConnectToServer(m_pClient->GetServerInfo(iServer));

			return (LOBBYMENU);
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

	return(HOSTMENU);
}

CTextManager* CHostGameMenu::GetTextManager() const
{
	return (m_pTextManager);
}