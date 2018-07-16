//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Button.cpp
// Description : Button class is used in the menu system to control highlighting, button clicks etc. (Holds a sprite)
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "system.h"

// This include
#include "Button.h"

// Static Variables

// Static Function Prototypes

// Implementation
CButton::CButton() : m_pSprite(nullptr), m_bHighlight(false)
{
	m_poPos.x = 0;
	m_poPos.y = 0;

	m_poOffset.x = 0;
	m_poOffset.y = 0;
}

CButton::~CButton()
{
	DeleteOBJ(m_pSprite);
}

bool CButton::Initialise(HWND _hWnd)
{
	m_hWnd = _hWnd;

	return true;
}

bool CButton::Initialise(HWND _hWnd, int _iResourceID, int _iHighlightID, int _iMaskResourceID)
{
	Initialise(_hWnd);

	m_iResourceID = _iResourceID;
	m_iHighlightID = _iHighlightID;
	m_iMaskResourceID = _iMaskResourceID;

	m_pSprite = new CSprite;
	VALIDATE(m_pSprite);

	VALIDATE(m_pSprite->Initialise(_iResourceID, _iMaskResourceID));

	return true;
}

POINT CButton::GetOffset() const
{
	return(m_poOffset);
}

void CButton::SetOffset(int _iX, int _iY)
{
	m_poOffset.x = _iX;
	m_poOffset.y = _iY;
}

POINT CButton::GetPosition() const
{
	return(m_poPos);
}

void CButton::SetPosition(int _iX, int _iY)
{
	m_poPos.x = _iX;
	m_poPos.y = _iY;

	if (m_pSprite)
	{
		m_pSprite->TranslateAbsolute(_iX, _iY);
	}
}

bool CButton::Process(bool& _rbIsHighlighted, bool _bAllowedToHighlight, bool _bAutoHighlight)
{
	bool bButtonClicked = false;

	POINT cursorPos;
	GetCursorPos(&cursorPos);
	ScreenToClient(m_hWnd, &cursorPos);	
	
	// Check if the user is hovering over one of the options
	if (CheckMenuHighlight(cursorPos, m_poPos, m_poOffset))
	{
		if (_bAllowedToHighlight)
		{
			if (_bAutoHighlight)
			{
				ManualHighlightOn();
			}
			
			_rbIsHighlighted = true;
		}

		// Process if the user has clicked on one of the buttons
		if(CSystemClass::GetInstance()->GetMouseState())
		{
			bButtonClicked = true;
		}
	}
	else
	{
		if (_rbIsHighlighted)
		{
			if (_bAutoHighlight)
			{
				ManualHighlightOff();
			}

			_rbIsHighlighted = false;
		}
	}

	return(bButtonClicked);
}

void CButton::Render()
{
	if(m_pSprite)
	{
		m_pSprite->Render();
	}
}

void CButton::ManualHighlightOn()
{
	m_pSprite->ChangeSprites(m_iHighlightID, m_iMaskResourceID);
	m_bHighlight = true;
}

void CButton::ManualHighlightOff()
{
	m_pSprite->ChangeSprites(m_iResourceID, m_iMaskResourceID);
	m_bHighlight = false;
}

bool CButton::CheckHighlight()
{
	return (m_bHighlight);
}