//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : TextManager.h
// Description : The text manager class manages text input and rendering of all text in the game.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__TEXTMANAGER_H_)
#define __TEXTMANAGER_H_

// Library Includes

// Local Includes
#include "utility.h"
#include "sprite.h"

// Types
struct tTextFieldInfo
{
	bool bRead;
	bool bWrite;
	bool bLetters;
	bool bNumbers;
	RECT rZone;
	CString strMessage;
	int iMaxLength;
	tColour colour;
};

// Constants

// Prototypes

// Class definition
class CTextManager
{
	// Member Functions
	public:
		CTextManager();
		~CTextManager();
		bool Initialise(HWND _hWnd, HDC _hDC, UINT _uiFieldNumber);

		void InputScreen(RECT _rArea, UINT _uiFieldNumber);
		void Process();
		void Render();

		void SetMessage(CString _strMessage, UINT _uiFieldNumber);
		CString GetString(UINT _uiFieldNumber);

		void SetupReadWrite(UINT _uiFieldNumber, bool _bRead, bool _bWrite, bool _bLetters, bool _bNumbers);
		void SetWriteAccess(UINT _uiFieldNumber, bool _bIsWritable);
		void SetKeyDown(bool _bKeyDown);
		void SetMaxLength(UINT _uiFieldNumber, UINT _uiMaxLength);
		void SetTextColour(UINT _uiFieldNumber, tColour _tColour);

		void SetFontSize(UINT _uiSize);
		void Select();

	protected:
		void RecordInput(UINT _uiFieldNumber);

	private:

	// Member Variables
	public:

	protected:

	private:
		std::vector<tTextFieldInfo> m_tFieldInfo;

		HWND	m_hWnd;
		HFONT	m_hFont;
		HDC		m_hDC;
		bool	m_bKeyDown, m_bCanAppend;
};

#endif // __TEXTMANAGER_H_