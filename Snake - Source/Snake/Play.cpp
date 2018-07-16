//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Play.cpp
// Description : The Play class manages the actual game processing, rendering etc.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "system.h"

// This include
#include "play.h"

// Static Variables

// Static Function Prototypes

// Implementation
CPlay::CPlay() : m_pBackground(nullptr), m_pSnake(nullptr), m_strGameOver(""), m_bGameOver(false), m_bLeavingGame(false), m_bToggle(true)
{
	for (int i = 0; i < 16; ++i)
	{
		m_pSnakes[i] = nullptr;
	}

	m_vSnakeInfo.clear();

	m_uiLocalScore = 0;
	m_uiHighScore = 0;

	m_fTimer = (float)clock();
	m_fLeaveTimer = 0.0f;
}

CPlay::~CPlay()
{
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pSnake);
	DeleteOBJ(m_pBackground);
	DeleteOBJ(m_pApple);

	for (UINT i = 0; i < m_vSnakeInfo.size(); ++i)
	{
		DeleteOBJ(m_vSnakeInfo[i]);
	}

	m_vSnakeInfo.clear();

	for (int i = 0; i < 16; ++i)
	{
		DeleteOBJ(m_pSnakes[i]);
	}
}

bool CPlay::Initialise()
{
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	if (!m_pBackground->Initialise(IDB_GAMESCREEN_SPRITE, IDB_BACKGROUND_MASK))
	{
		MessageBox(NULL, L"Can't create game background sprite", L"ERROR", MB_OK);
		return false;
	}

	m_pBackground->TranslateAbsolute(512, 360);

	m_pSnake = new CSnake;
	VALIDATE(m_pSnake);

	if (!m_pSnake->Initialise())
	{
		MessageBox(NULL, L"Can't create snake", L"ERROR", MB_OK);
		return false;
	}

	for (int i = 0; i < 16; ++i)
	{
		m_pSnakes[i] = new CSprite;
		VALIDATE(m_pSnakes[i]);

		if (!m_pSnakes[i]->Initialise(IDB_SNAKEWHITE_SPRITE + i, IDB_SNAKE_MASK))
		{
			MessageBox(NULL, L"Can't create other snake sprites", L"ERROR", MB_OK);
			return false;
		}
	}

	m_pApple = new CSprite;
	VALIDATE(m_pApple);

	if (!m_pApple->Initialise(IDB_APPLE_SPRITE, IDB_SNAKE_MASK))
	{
		MessageBox(NULL, L"Can't create game apple sprite", L"ERROR", MB_OK);
		return false;
	}

	m_pClient = CSystemClass::GetInstance()->GetGameInstance()->GetClient();
	m_hDC = CSystemClass::GetInstance()->GetBackBuffer()->GetBFDC();

	SelectObject(m_hDC, CreateFont(25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"starcraft"));

	return true;
}

void CPlay::Render()
{
	//Render background
	m_pBackground->Render();

	if (!m_bGameOver)
	{
		// Render apples
		for (UINT i = 0; i < m_vApplePos.size(); ++i)
		{
			int iX = m_vApplePos[i].x;
			int iY = m_vApplePos[i].y;
			m_pApple->TranslateAbsolute(iX, iY);
			m_pApple->Render();
		}
	
		// Render our snake
		if(m_pSnake->IsAlive())
		{
			m_pSnake->Render();
		}
	
		// Render other snakes
		for (UINT i = 0; i < m_vSnakeInfo.size(); ++i) 
		{
			if (m_vSnakeInfo[i]->bAlive)
			{
				for (UINT j = 0; j < m_vSnakeInfo[i]->tSnakePos.size(); ++j)
				{
					int iX = int(floor(m_vSnakeInfo[i]->tSnakePos[j].fX + 0.5f));
					int iY = int(floor(m_vSnakeInfo[i]->tSnakePos[j].fY + 0.5f));
					m_pSnakes[m_vSnakeInfo[i]->ePlayerColour]->TranslateAbsolute(iX, iY);
					m_pSnakes[m_vSnakeInfo[i]->ePlayerColour]->Render();
				}

				for (UINT j = 0; j < m_vSnakeInfo[i]->tTurningPoints.size(); ++j)
				{
					int iX = int(m_vSnakeInfo[i]->tTurningPoints[j].fX);
					int iY = int(m_vSnakeInfo[i]->tTurningPoints[j].fY);
					m_pSnakes[m_vSnakeInfo[i]->ePlayerColour]->TranslateAbsolute(iX, iY);
					m_pSnakes[m_vSnakeInfo[i]->ePlayerColour]->Render();
				}
			}
		}

		if (!m_pSnake->IsAlive())
		{
			RECT R = {(SCREENWIDTH / 2) - 75, SCREENHEIGHT - 100, 0, 0};
			DrawText(m_hDC, L"SPECTATING", -1, &R, DT_NOCLIP);
		}

		RenderScores();
	}
	else
	{
		RECT R = {100, 100, 0, 0};
		CStringW strGameOver = m_strGameOver;
		DrawText(m_hDC, strGameOver, -1, &R, DT_NOCLIP);
	}

	if(m_bLeavingGame)
	{
		RECT R = {15, SCREENHEIGHT - 60, 0, 0};
		CStringW strGameOver = "Push ESC again to leave the current game";
		DrawText(m_hDC, strGameOver, -1, &R, DT_NOCLIP);
	}
}

void CPlay::RenderScores()
{
	CStringA str = "Your Score: ";
	str.AppendFormat("%d", m_uiLocalScore);
	CStringW str2 = str;

	RECT R = {20, 2, 0, 0};
	DrawText(m_hDC, str2, -1, &R, DT_NOCLIP);

	str = "Leader Score: ";
	str.AppendFormat("%d", m_uiHighScore);
	str2 = str;

	RECT R2 = {580, 2, 0, 0};
	DrawText(m_hDC, str2, -1, &R2, DT_NOCLIP);
}

void CPlay::Process(float _fDeltaTick)
{
	// Process the player's snake (movement etc)
	if (!m_bGameOver)
	{
		m_pSnake->Process(_fDeltaTick);

		// Sends off its updated information
		tPointF poPosition = m_pSnake->GetSnakePos();
		if(poPosition.fX != -10000 && poPosition.fY != -10000)
		{
			m_pClient->SendSnakeInfo(m_pSnake->GetSnakeInfo());
		}

		if (clock() - m_fTimer > 500.0f && m_pSnake->IsAlive())
		{
			m_uiLocalScore += 1;
			m_fTimer = (float)clock();
			m_pClient->SendScore(m_uiLocalScore);
		}

		if(GetAsyncKeyState(VK_ESCAPE) && m_bToggle)
		{
			m_bToggle = false;

			if(!m_bLeavingGame)
			{
				m_bLeavingGame = true;
				m_fLeaveTimer = (float)clock();
			}
			else
			{
				// LEAVE GAME
				m_bLeavingGame = false;
				m_pClient->SendDisconnectNotice();
				CSystemClass::GetInstance()->GetGameInstance()->SetState(MENU);
				return;
			}
		}	

		if(m_bLeavingGame && clock() - m_fLeaveTimer > 3000.0f)
		{
			m_bLeavingGame = false;		
		}
	}
	else
	{
		// End game timer
		if (clock() - m_fTimer > 10000.0f)
		{
			CSystemClass::GetInstance()->GetGameInstance()->SetState(MENU);
		}
	}
}

void CPlay::AddToSnakeList(tGameInfo _tSnakeInfo, bool _bPlayer)
{
	bool bAdd = true;

	for (UINT i = 0; i < m_vSnakeInfo.size(); ++i)
	{
		if (_tSnakeInfo.ePlayerColour == m_vSnakeInfo[i]->ePlayerColour)
		{
			bAdd = false;

			m_vSnakeInfo[i]->tSnakePos.clear();
			m_vSnakeInfo[i]->tTurningPoints.clear();

			// Update info
			m_vSnakeInfo[i]->tSnakePos = _tSnakeInfo.tSnakePos;
			m_vSnakeInfo[i]->tTurningPoints = _tSnakeInfo.tTurningPoints;

			break;
		}
	}

	if(_bPlayer && m_pSnake->GetSnakePos().fX != -10000.0f && m_pSnake->GetSnakePos().fY != -10000.0f)
	{
		bAdd = false;
	}

	if (bAdd)
	{
		if(_bPlayer)
		{
			m_pSnake->CreateSnake(_tSnakeInfo.tSnakePos[0]);
		}
		else
		{
			tGameInfo* pNewInfo = new tGameInfo;

			pNewInfo->tSnakePos = _tSnakeInfo.tSnakePos;
			pNewInfo->ePlayerColour = _tSnakeInfo.ePlayerColour;
			pNewInfo->bAlive = true;

			m_vSnakeInfo.push_back(pNewInfo);
		}
	}
}

void CPlay::AddToAppleList(std::vector<POINT> _vApplePos)
{
	m_vApplePos.clear();
	m_vApplePos = _vApplePos;
}

void CPlay::RemoveFromAppleList(POINT _poPosition)
{
	float fDistance = 99999.0f;
	int iClosestIndex = 0;

	// Search through the apples and find the closest one
	for(UINT i = 0; i < m_vApplePos.size(); ++i)
	{
		float fX = float(m_vApplePos[i].x - _poPosition.x);
		float fY = float(m_vApplePos[i].y - _poPosition.y);
		float fNewDistance = sqrt((fX * fX) + (fY * fY));

		if(fNewDistance < fDistance)
		{
			fDistance = fNewDistance;
			iClosestIndex = i;
		}
	}

	m_vApplePos.erase(m_vApplePos.begin() + iClosestIndex);

	m_pClient->SendApple(_poPosition);
}

void CPlay::DisconnectionMessageFromUserLeavingTheGameDuringGameInProgess(PLAYERCOLOURS _ePlayerColour) // fuck this project. gimme A+ plz
{
	for (UINT i = 0; i < m_vSnakeInfo.size(); ++i)
	{
		if (m_vSnakeInfo[i]->ePlayerColour == _ePlayerColour)
		{
			m_vSnakeInfo.erase(m_vSnakeInfo.begin() + i);
		}
	}
}

void CPlay::DeathNotice(PLAYERCOLOURS _ePlayerColour)
{
	for (UINT i = 0; i < m_vSnakeInfo.size(); ++i)
	{
		if (_ePlayerColour == m_vSnakeInfo[i]->ePlayerColour)
		{
			// Update info
			m_vSnakeInfo[i]->bAlive = false;
			break;
		}
	}
}

void CPlay::AddAppleScore()
{
	m_uiLocalScore += 100;

	// Update the server with our score
	m_pClient->SendScore(m_uiLocalScore);
}

void CPlay::SetHighScore(UINT _uiScore)
{
	m_uiHighScore = _uiScore;
}

void CPlay::GameOver(CStringA _strLeader, bool _bYouWin)
{
	m_strGameOver = "Game Over!\n";
	m_strGameOver.Append(_bYouWin ? "Congratulations you Won!\n\n" : "Bad luck, you Lost!\n\n");
	m_strGameOver.Append(_strLeader);
	m_strGameOver.Append(" has won with ");
	m_strGameOver.AppendFormat("%d", m_uiHighScore);
	m_strGameOver.Append(" points!");
	m_strGameOver.Append("\n\n\nReturning to the menu...");

	m_bGameOver = true;

	m_fTimer = (float)clock();
}

void CPlay::SetKeyDown(bool _bKeyDown)
{
	m_bKeyDown = _bKeyDown;

	if (!m_bKeyDown)
	{
		m_bToggle = true;
	}
}