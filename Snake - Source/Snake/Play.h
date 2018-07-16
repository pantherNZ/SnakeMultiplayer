//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Play.h
// Description : The Play class manages the actual game processing, rendering etc.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//


#pragma once

#if !defined(__PLAY_H__)
#define __PLAY_H__

// Library Includes

// Local Includes
#include "utility.h"
#include "Sprite.h"
#include "resource.h"
#include "Snake.h"

// Types

// Constants
class CClient;

// Prototypes
class CPlay
{
	// Member Functions
	public:
		CPlay();
		~CPlay();

		bool Initialise();

		void Render();
		void Process(float _fDeltaTick);
		void AddToSnakeList(tGameInfo _tSnakeInfo, bool _bPlayer);

		void RenderScores();

		void AddToAppleList(std::vector<POINT> _vApplePos);
		void RemoveFromAppleList(POINT _poPosition);

		void DeathNotice(PLAYERCOLOURS _ePlayerColour);

		void AddAppleScore();
		void SetHighScore(UINT _uiScore);

		void GameOver(CStringA _strLeader, bool _bYouWin);
		void SetKeyDown(bool _bKeyDown);

		void DisconnectionMessageFromUserLeavingTheGameDuringGameInProgess(PLAYERCOLOURS _ePlayerColour);

	protected:

	private:
		void RenderSnake(tGameInfo _tSnakeInfo, CSprite* _pSnakeSprite);
		int RenderBetweenPositions(CSprite* _pSnakeSprite, POINT _p1, POINT _p2, SNAKEDIRECTION _eTempDirection, bool _bDrawMovingTailSquare, int _iLeftToRender, int _iTailOffset);

	// Member Variables
	public:

	protected:

	private:
		HDC							m_hDC;
		CClient*					m_pClient;
		CSprite*					m_pBackground;
		CSnake*						m_pSnake;
		std::vector<tGameInfo*>		m_vSnakeInfo;

		CSprite*					m_pSnakes[16];
		CSprite*					m_pApple;
		std::vector<POINT>			m_vApplePos;

		UINT						m_uiLocalScore; 
		UINT						m_uiHighScore;
		float						m_fTimer;
		float						m_fLeaveTimer;
		bool						m_bGameOver;
		bool						m_bLeavingGame;
		bool						m_bKeyDown;
		bool						m_bToggle;
		CStringA					m_strGameOver;
};

#endif // __PLAY_H__