//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Snake.cpp
// Description : Snake class is a subclass of CPlay and solely manages the player's own movement, processing and rendering etc.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes
#include <math.h>
// Local Includes
#include "utility.h"

// This Include
#include "Snake.h"
#include "System.h"

// Implementation
CSnake::CSnake() : m_pSnakeSprite(nullptr), m_fMoveSpeed(150.0f), m_eWaitingToTurn(RIGHT)
{
	m_tSnakeInfo.bAlive = true;
	m_tSnakeInfo.tSnakePos.push_back(tPointF(-10000.0f, -10000.0f, RIGHT, -1));
}

CSnake::~CSnake()
{
	DeleteOBJ(m_pSnakeSprite);

	m_tSnakeInfo.tSnakePos.clear();
}

bool CSnake::Initialise()
{
	m_pSnakeSprite = new CSprite;
	VALIDATE(m_pSnakeSprite);

	m_tSnakeInfo.ePlayerColour = CSystemClass::GetInstance()->GetGameInstance()->GetClient()->GetColour();
	int iColourNum = m_tSnakeInfo.ePlayerColour;

	if (!m_pSnakeSprite->Initialise(IDB_SNAKEWHITE_SPRITE + iColourNum, IDB_SNAKE_MASK))
	{
		MessageBox(NULL, L"Can't create game snake sprite", L"ERROR", MB_OK);
		return false;
	}

	m_hDC = CSystemClass::GetInstance()->GetBackBuffer()->GetBFDC();

	return true;
}
 
void CSnake::Render()
{
	for (UINT i = 0; i < m_tSnakeInfo.tSnakePos.size(); ++i)
	{
		int iX = int(floor(m_tSnakeInfo.tSnakePos[i].fX + 0.5f));
		int iY = int(floor(m_tSnakeInfo.tSnakePos[i].fY + 0.5f));
		m_pSnakeSprite->TranslateAbsolute(iX, iY);
		m_pSnakeSprite->Render();
	}

	for (UINT i = 0; i < m_tSnakeInfo.tTurningPoints.size(); ++i)
	{
		m_pSnakeSprite->TranslateAbsolute((int)m_tSnakeInfo.tTurningPoints[i].fX, (int)m_tSnakeInfo.tTurningPoints[i].fY);
		m_pSnakeSprite->Render();
	}
}

void CSnake::Process(float _fDeltaTick)
{
	if (m_tSnakeInfo.bAlive && m_tSnakeInfo.tSnakePos.size() > 0)
	{
		if (m_tSnakeInfo.tSnakePos[0].fX != -10000.0f && m_tSnakeInfo.tSnakePos[0].fY != -10000.0f)
		{
			CollisionDetection();	
		}

		ProcessInput();	

		int iX = int(floor(m_tSnakeInfo.tSnakePos[0].fX + 0.5f - BOUNDARYOFFSET));
		int iY = int(floor(m_tSnakeInfo.tSnakePos[0].fY + 0.5f - BOUNDARYOFFSET));
		float fXRound = floor(m_tSnakeInfo.tSnakePos[0].fX + 0.5f);
		float fYRound = floor(m_tSnakeInfo.tSnakePos[0].fY + 0.5f);
		bool bTurn = false;

		if (iX % GRIDSIZE == 0 && iY % GRIDSIZE == 0 && m_eWaitingToTurn != m_tSnakeInfo.tSnakePos[0].eDirection)
		{
			
			m_tSnakeInfo.tTurningPoints.push_back(tPointF(fXRound, fYRound, m_eWaitingToTurn, -1));
			m_tSnakeInfo.tSnakePos[0].eDirection = m_eWaitingToTurn;

			for (UINT i = 0; i < m_tSnakeInfo.tSnakePos.size(); ++i)
			{
				if(m_tSnakeInfo.tSnakePos[i].iTurningPointNumber == -1)
				{
					m_tSnakeInfo.tSnakePos[i].iTurningPointNumber = m_tSnakeInfo.tTurningPoints.size() - 1;
				}
			}

			bTurn = true;
		}

		for (UINT i = 0; i < m_tSnakeInfo.tSnakePos.size(); ++i)
		{
			float fGap = (i == 0 && bTurn ? abs(fXRound - m_tSnakeInfo.tSnakePos[0].fX) + abs(fYRound - m_tSnakeInfo.tSnakePos[0].fY): 0);

			switch (m_tSnakeInfo.tSnakePos[i].eDirection)
			{
				case (UP):
				{
					m_tSnakeInfo.tSnakePos[i].fY -= ((m_fMoveSpeed * _fDeltaTick) - fGap);
				}
				break;

				case (RIGHT):
				{
					m_tSnakeInfo.tSnakePos[i].fX += ((m_fMoveSpeed * _fDeltaTick) - fGap);
				}
				break;

				case (DOWN):
				{
					m_tSnakeInfo.tSnakePos[i].fY += ((m_fMoveSpeed * _fDeltaTick) - fGap);
				}
				break;

				case (LEFT):
				{
					m_tSnakeInfo.tSnakePos[i].fX -= ((m_fMoveSpeed * _fDeltaTick) - fGap);
				}
				break;

				default: break;
			};
		
			float fX = m_tSnakeInfo.tSnakePos[i].fX;
			float fY = m_tSnakeInfo.tSnakePos[i].fY;

			if (m_tSnakeInfo.tTurningPoints.size() > 0 && i > 0 && m_tSnakeInfo.tSnakePos[i].iTurningPointNumber != -1)
			//if (m_tSnakeInfo.tTurningPoints.size() > 0 && m_tSnakeInfo.tSnakePos[i].iTurningPointNumber != -1)
			{
				float fTurnX = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fX;
				float fTurnY = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fY;

				// Check if within a range of the future turning point 
				if (abs(fTurnX - fX) <= 20.0f && abs(fTurnY - fY) <= 20.0f)
				{
					bool bPast = false;
					float fDiffX;
					float fDiffY;

					switch(m_tSnakeInfo.tSnakePos[i].eDirection)
					{
						case (UP):
						{
							bPast = (fY <= fTurnY);
							fDiffX = 0;
							fDiffY = abs(fY - fTurnY);
						}
						break;

						case (RIGHT):
						{
							bPast = (fX >= fTurnX);
							fDiffX = abs(fX - fTurnX);
							fDiffY = 0;
						}
						break;

						case (DOWN):
						{
							bPast = (fY >= fTurnY);
							fDiffX = 0;
							fDiffY = abs(fTurnY - fY);
						}
						break;

						case (LEFT):
						{
							bPast = (fX <= fTurnX);
							fDiffX = abs(fTurnX - fX);
							fDiffY = 0;
						}					
						break;

						default: break;
					}

					if(bPast)
					{
						m_tSnakeInfo.tSnakePos[i].eDirection = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].eDirection;
					
						switch(m_tSnakeInfo.tSnakePos[i].eDirection)
						{
							case (UP):
							{
								m_tSnakeInfo.tSnakePos[i].fX = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fX;
								m_tSnakeInfo.tSnakePos[i].fY = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fY - fDiffX;
							}
							break;

							case (RIGHT):
							{
								m_tSnakeInfo.tSnakePos[i].fX = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fX + fDiffY;
								m_tSnakeInfo.tSnakePos[i].fY = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fY;
							}
							break;

							case (DOWN):
							{
								m_tSnakeInfo.tSnakePos[i].fX = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fX;
								m_tSnakeInfo.tSnakePos[i].fY = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fY + fDiffX;
							}
							break;

							case (LEFT):
							{
								m_tSnakeInfo.tSnakePos[i].fX = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fX - fDiffY;
								m_tSnakeInfo.tSnakePos[i].fY = m_tSnakeInfo.tTurningPoints[m_tSnakeInfo.tSnakePos[i].iTurningPointNumber].fY;
							}					
							break;

							default: break;
						}

						if(i == m_tSnakeInfo.tSnakePos.size() - 1)
						{
							m_tSnakeInfo.tTurningPoints.erase(m_tSnakeInfo.tTurningPoints.begin() + m_tSnakeInfo.tSnakePos[i].iTurningPointNumber);
							m_tSnakeInfo.tSnakePos[i].iTurningPointNumber = -1;
							
							if(m_tSnakeInfo.tTurningPoints.size() > 0)
							{
								for (UINT i = 1; i < m_tSnakeInfo.tSnakePos.size(); ++i)
								{
									if (m_tSnakeInfo.tSnakePos[i].iTurningPointNumber != -1)
									{
										--m_tSnakeInfo.tSnakePos[i].iTurningPointNumber;
									}
								}

								++m_tSnakeInfo.tSnakePos[m_tSnakeInfo.tSnakePos.size() - 1].iTurningPointNumber;
							}			

							return;
						}

						// Direct the snake to the next turning number or set it to -1 if there are no more turning points
						if (m_tSnakeInfo.tSnakePos[i].iTurningPointNumber == (m_tSnakeInfo.tTurningPoints.size() - 1))
						{
							m_tSnakeInfo.tSnakePos[i].iTurningPointNumber = -1;
						}
						else
						{
							++m_tSnakeInfo.tSnakePos[i].iTurningPointNumber;
						}		
					}
				}
			}
		}
	}
}

void CSnake::ProcessInput()
{
	if (GetAsyncKeyState(VK_LEFT) && m_eWaitingToTurn != RIGHT && m_tSnakeInfo.tSnakePos[0].eDirection != RIGHT)			// Left Movement
	{
		m_eWaitingToTurn = LEFT;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && m_eWaitingToTurn != LEFT && m_tSnakeInfo.tSnakePos[0].eDirection != LEFT)	// Right Movement
	{
		m_eWaitingToTurn = RIGHT;
	}
	else if (GetAsyncKeyState(VK_UP) && m_eWaitingToTurn != DOWN && m_tSnakeInfo.tSnakePos[0].eDirection != DOWN)		// Up Movement
	{
		m_eWaitingToTurn = UP;
	}
	else if (GetAsyncKeyState(VK_DOWN) && m_eWaitingToTurn != UP && m_tSnakeInfo.tSnakePos[0].eDirection != UP)		// Down Movement
	{
		m_eWaitingToTurn = DOWN;
	}
}

tPointF CSnake::GetSnakePos() const
{
	return (m_tSnakeInfo.tSnakePos[0]);
}

void CSnake::CreateSnake(tPointF _tPosition)
{
	m_tSnakeInfo.tSnakePos[0] = _tPosition;
	m_eWaitingToTurn = _tPosition.eDirection;

	UINT uiLength = 6;

	for (UINT i = 1; i < uiLength; ++i)
	{
		// Move in the direction that has been initialised
		MoveInDirection(_tPosition, SNAKEDIRECTION(_tPosition.eDirection * -1));

		// Add the new position to the snake
		m_tSnakeInfo.tSnakePos.push_back(_tPosition);
	}
}

void CSnake::MoveInDirection(tPointF& _rtNewPosition, SNAKEDIRECTION _eDirection)
{
	switch (_eDirection)
	{
		case (UP):
		{
			_rtNewPosition.fY -= 19;
		}
		break;

		case (RIGHT):
		{
			_rtNewPosition.fX += 19;
		}
		break;

		case (DOWN):
		{
			_rtNewPosition.fY += 19;
		}
		break;

		case (LEFT):
		{
			_rtNewPosition.fX -= 19;
		}
		break;

		default: break;
	};
}

bool CSnake::IsAlive() const
{
	return (m_tSnakeInfo.bAlive);
}

tGameInfo CSnake::GetSnakeInfo() const
{
	return(m_tSnakeInfo);
}

CSprite* CSnake::GetSprite() const
{
	return(m_pSnakeSprite);
}

void CSnake::CollisionDetection()
{
	COLORREF crBackground = RGB(0, 0, 0);
	COLORREF crApple = RGB(251, 0, 0);
	COLORREF crCheckSpotLeft, crCheckSpotRight, crCheckSpotMiddle;

	float fXLeft = m_tSnakeInfo.tSnakePos[0].fX;
	float fYLeft = m_tSnakeInfo.tSnakePos[0].fY;
	
	float fXRight = m_tSnakeInfo.tSnakePos[0].fX;
	float fYRight = m_tSnakeInfo.tSnakePos[0].fY;

	float fXMiddle = m_tSnakeInfo.tSnakePos[0].fX;
	float fYMiddle = m_tSnakeInfo.tSnakePos[0].fY;

	switch (m_tSnakeInfo.tSnakePos[0].eDirection)
	{
		case (UP):
		{
			// Left Spot
			fXLeft -= 9.0f;
			fYLeft -= 12.0f;

			// Right Spot
			fXRight += 9.0f;
			fYRight -= 12.0f;

			// Middle Spot
			fYMiddle -= GRIDSIZE;
		}
		break;

		case (DOWN):
		{
			// Left Spot
			fXLeft -= 9.0f;
			fYLeft += 12.0f;

			// Right Spot
			fXRight += 9.0f;
			fYRight += 12.0f;

			// Middle Spot
			fYMiddle += GRIDSIZE;
		}
		break;

		case (LEFT):
		{
			// Left Spot
			fXLeft -= 12.0f;
			fYLeft += 9.0f;

			// Right Spot
			fXRight -= 12.0f;
			fYRight -= 9.0f;

			// Middle Spot
			fXMiddle -= GRIDSIZE;
		}
		break;

		case (RIGHT):
		{
			// Left Spot
			fXLeft += 12.0f;
			fYLeft -= 9.0f;

			// Right Spot
			fXRight += 12.0f;
			fYRight += 9.0f;

			// Middle Spot
			fXMiddle += GRIDSIZE;
		}
		break;

		default: break;
	};

	crCheckSpotLeft = GetPixel(m_hDC, int(fXLeft), int(fYLeft));
	crCheckSpotRight = GetPixel(m_hDC, int(fXRight), int(fYRight));

	if (crCheckSpotLeft != crBackground || crCheckSpotRight != crBackground) 
	{
		m_tSnakeInfo.bAlive = false;

		// The snake has collided with something
		CSystemClass::GetInstance()->GetGameInstance()->GetClient()->SendDeathNotice();
	}
	else
	{
		crCheckSpotMiddle = GetPixel(m_hDC, int(fXMiddle), int (fYMiddle));

		// Check if player hit an apple
		if (crCheckSpotMiddle == crApple)
		{
			// Set the pixel to black to avoid further collisions that have already happened
			SetPixel(m_hDC, int(fXMiddle), int(fYMiddle), RGB(0, 0, 0));

			// Add one to the end of the snake
			// Find the last piece of the snake's direction & inverse is 
			tPointF tNewPosition = m_tSnakeInfo.tSnakePos[m_tSnakeInfo.tSnakePos.size() - 1];
			SNAKEDIRECTION eDirection = (SNAKEDIRECTION)(tNewPosition.eDirection * -1);

			// Move the new spot back from the end
			MoveInDirection(tNewPosition, eDirection);

			// Add it to the vector
			m_tSnakeInfo.tSnakePos.push_back(tNewPosition);

			POINT p = {(long)m_tSnakeInfo.tSnakePos[0].fX, (long)m_tSnakeInfo.tSnakePos[0].fY};
			// Tell the play class to remove the apple that was hit
			CSystemClass::GetInstance()->GetGameInstance()->GetPlay()->RemoveFromAppleList(p);

			// Add to our score
			CSystemClass::GetInstance()->GetGameInstance()->GetPlay()->AddAppleScore();
		}
	}
}