//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Timer.cpp
// Description : The timer class is used to calculate game time and delta tick
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "timer.h"

// Static Variables

// Static Function Prototypes

// Implementation
CTimer::CTimer(): m_fTimeElapsed(0.0f), m_fDeltaTime(0.0f), m_fLastTime(0.0f), m_fCurrentTime(0.0f)
{

}

CTimer::~CTimer()
{

}

void CTimer::Process()
{
	m_fLastTime = m_fCurrentTime;
	m_fCurrentTime = static_cast<float>(clock());

	if (m_fLastTime == 0.0f)
	{
		m_fLastTime = m_fCurrentTime;
	}

	m_fDeltaTime = m_fCurrentTime - m_fLastTime;
	m_fTimeElapsed += m_fDeltaTime;
}

float CTimer::GetDeltaTick() const
{
	return (m_fDeltaTime / 1000.0f);
}

float CTimer::GetGameTime() const
{
	return(m_fTimeElapsed);
}