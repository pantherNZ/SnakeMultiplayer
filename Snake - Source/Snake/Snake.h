//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Snake.h
// Description : Snake class is a subclass of CPlay and solely manages the player's own movement, processing and rendering etc.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__SNAKE_H__)
#define __SNAKE_H__

// Library Includes

// Local Includes
#include "Utility.h"
#include "Sprite.h"

// Constants

class CSnake
{
	// Member Functions
	public:
		CSnake();
		~CSnake();

		bool Initialise();

		void Process(float _fDeltaTick);
		void Render();

		void CreateSnake(tPointF _tPosition);
		tPointF GetSnakePos() const;

		tGameInfo GetSnakeInfo() const;
		bool IsAlive() const;
		
		CSprite* GetSprite() const;

	protected:

	private:
		void CollisionDetection();
		void ProcessInput();
		void MoveInDirection(tPointF& _rtNewPosition, SNAKEDIRECTION _eDirection);

	// Member Variables
	public:

	protected:

	private:
		HDC						m_hDC;
		SNAKEDIRECTION			m_eWaitingToTurn;
		float					m_fMoveSpeed;
		CSprite*				m_pSnakeSprite;
		tGameInfo				m_tSnakeInfo;	
};

#endif // __SNAKE_H__