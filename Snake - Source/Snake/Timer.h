//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Timer.h
// Description : The timer class is used to calculate game time and delta tick
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(_TIMER_H__)
#define _TIMER_H__

// Library Includes

// Local Includes
#include "Utility.h"

// Types

// Constants

// Prototypes
class CTimer
{
	// Member Functions
	public:
		CTimer();
		~CTimer();
		void Process();
		float GetDeltaTick() const;
		float GetGameTime() const;

	protected:

	private:


	// Member Variables
	public:

	protected:
		float m_fTimeElapsed;
		float m_fDeltaTime;
		float m_fLastTime;
		float m_fCurrentTime;

	private:

};

#endif // _TIMER_H__