//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : TextManager.cpp
// Description : The text manager class manages text input and rendering of all text in the game.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes

// This include
#include "TextManager.h"

// Static Variables

// Static Function Prototypes

// Implementation
CTextManager::CTextManager() : m_bCanAppend(true), m_bKeyDown(false)
{

}

CTextManager::~CTextManager()
{
	DeleteObject(m_hFont);
	ReleaseDC(m_hWnd, m_hDC);
}

bool CTextManager::Initialise(HWND _hWnd, HDC _hDC, UINT _uiFieldCount)
{
	m_hDC = _hDC;
	m_hWnd = _hWnd;
	ReleaseDC(m_hWnd, m_hDC);

	SetFontSize(20);

	// Create zones and strings and bools for each of _i

	for (UINT i = 0; i < _uiFieldCount; ++i)
	{
		tTextFieldInfo tTemp;

		tTemp.bRead = false;
		tTemp.bWrite = false;
		RECT rTemp = {0, 0, 0, 0};
		tTemp.rZone = rTemp;
		tTemp.strMessage = "";
		tTemp.iMaxLength = 25;
		tTemp.colour = tColour(255, 255, 255, WHITE);

		m_tFieldInfo.push_back(tTemp);
	}

	return (true);
}

void CTextManager::InputScreen(RECT _rArea, UINT _uiFieldNumber)
{
	m_tFieldInfo[_uiFieldNumber].rZone = _rArea;
}

void CTextManager::Render()
{
	for (unsigned int i = 0; i < m_tFieldInfo.size(); ++i)
	{
		if (m_tFieldInfo[i].bRead)
		{
			SetTextColor(m_hDC, RGB(m_tFieldInfo[i].colour.iR, m_tFieldInfo[i].colour.iG, m_tFieldInfo[i].colour.iB));
			DrawText(m_hDC, m_tFieldInfo[i].strMessage, -1, &m_tFieldInfo[i].rZone, DT_NOCLIP);
		}
	}
}

void CTextManager::Process()
{
	for (unsigned int i = 0; i < m_tFieldInfo.size(); ++i)
	{
		if (m_tFieldInfo[i].bWrite)
		{
			RecordInput(i);
		}
	}
}

void CTextManager::SetMessage(CString _strMessage, UINT _uiFieldNumber)
{
	m_tFieldInfo[_uiFieldNumber].strMessage = _strMessage;
}

CString CTextManager::GetString(UINT _uiFieldNumber)
{
	return (m_tFieldInfo[_uiFieldNumber].strMessage);
}

void CTextManager::RecordInput(UINT _uiFieldNumber)
{
	// Record what the user is typing in and append it to the string for displaying	
	
	if (m_bCanAppend && m_bKeyDown)
	{
		if (m_tFieldInfo[_uiFieldNumber].bLetters)  // Do we want to check the letters?
		{
			for (int i = 65; i < 91; ++i)	// Check A - Z
			{
				if (GetAsyncKeyState(i))
				{
					if (m_tFieldInfo[_uiFieldNumber].strMessage.GetLength() < m_tFieldInfo[_uiFieldNumber].iMaxLength)
					{
						m_tFieldInfo[_uiFieldNumber].strMessage.AppendChar(i);
						m_bCanAppend = false;
						break;
					}
				}
			}
		}

		if (m_tFieldInfo[_uiFieldNumber].bNumbers) // Do we want to check the numbers?
		{
			for (int i = 48; i < 58; ++i)	// Check 0 - 9
			{
				if (GetAsyncKeyState(i))
				{
					if (m_tFieldInfo[_uiFieldNumber].strMessage.GetLength() < m_tFieldInfo[_uiFieldNumber].iMaxLength)
					{
						m_tFieldInfo[_uiFieldNumber].strMessage.AppendChar(i);
						m_bCanAppend = false;
						break;
					}
				}
			}
		}

		if (GetAsyncKeyState(VK_SPACE)) // Check if they hit space
		{
			if (m_tFieldInfo[_uiFieldNumber].strMessage.GetLength() < m_tFieldInfo[_uiFieldNumber].iMaxLength)
			{
				m_tFieldInfo[_uiFieldNumber].strMessage.AppendChar(' ');
				m_bCanAppend = false;
			}
		}

		if (GetAsyncKeyState(VK_BACK)) // Check if they want to remove the last character they entered // Restriction: Have to hit backspace for every character
		{
			m_tFieldInfo[_uiFieldNumber].strMessage.Delete(m_tFieldInfo[_uiFieldNumber].strMessage.GetLength() - 1, 1);
			m_bCanAppend = false;
		}
	}
}

void CTextManager::SetupReadWrite(UINT _uiFieldNumber, bool _bRead, bool _bWrite, bool _bLetters, bool _bNumbers)
{
	m_tFieldInfo[_uiFieldNumber].bLetters = _bLetters;
	m_tFieldInfo[_uiFieldNumber].bNumbers = _bNumbers;
	m_tFieldInfo[_uiFieldNumber].bRead = _bRead;
	m_tFieldInfo[_uiFieldNumber].bWrite = _bWrite;
}

void CTextManager::SetWriteAccess(UINT _uiFieldNumber, bool _bIsWritable)
{
	m_tFieldInfo[_uiFieldNumber].bWrite = _bIsWritable;
}

void CTextManager::SetKeyDown(bool _bKeyDown)
{
	m_bKeyDown = _bKeyDown;

	if (!m_bKeyDown)
	{
		m_bCanAppend = true;
	}
}

void CTextManager::SetMaxLength(UINT _uiFieldNumber, UINT _uiMaxLength)
{
	m_tFieldInfo[_uiFieldNumber].iMaxLength = _uiMaxLength;
}

void CTextManager::SetFontSize(UINT _uiSize)
{
	DeleteObject(m_hFont);

	m_hFont = CreateFont(_uiSize, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"starcraft");
	//SetTextColor(m_hDC, RGB(255, 255, 255));
	SetBkMode(m_hDC, TRANSPARENT);
}

void CTextManager::Select()
{
	SelectObject(m_hDC, m_hFont);
}

void CTextManager::SetTextColour(UINT _uiFieldNumber, tColour _tColour)
{
	m_tFieldInfo[_uiFieldNumber].colour = _tColour;
}