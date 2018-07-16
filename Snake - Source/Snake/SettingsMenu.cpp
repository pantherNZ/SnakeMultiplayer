// Library Includes

// Local Includes
#include "resource.h"

// This Include
#include "SettingsMenu.h"

// Static Variables

// Static Function Prototypes

// Implementation
CSettingsMenu::CSettingsMenu() : m_pBackground(nullptr)
{

}

CSettingsMenu::~CSettingsMenu()
{
	DeleteOBJ(m_pBackground);
}

bool CSettingsMenu::Initialise(HWND _hWnd)
{
	m_hWnd = _hWnd;

	// Create our background sprite
	m_pBackground = new CSprite;
	VALIDATE(m_pBackground);

	// Initialise the background sprite
	VALIDATE(m_pBackground->Initialise(IDB_BACKGROUND_MASK, IDB_BACKGROUND_MASK));

	m_pBackground->TranslateAbsolute(512, 360);

	return(true);
}

void CSettingsMenu::Render()
{
	m_pBackground->Render();
}

MENUSTATE CSettingsMenu::Process()
{

	return(SETTINGSMENU);
}
