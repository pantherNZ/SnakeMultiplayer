//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : System.h
// Description : The system class is top class of the program which manages the screen, the game loop and Windows callback function. Contains all other classes.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#ifndef _CSYSTEMCLASS_H_
#define _CSYSTEMCLASS_H_

// Definitions
#define WIN32_LEAN_AND_MEAN

// Library Includes:

// Local Includes:
#include "utility.h"
#include "timer.h"
#include "game.h"

class CHostGameMenu;
class CJoinGameMenu;

// Class Declaration:
class CSystemClass
{
	// Member Functions
	public:	
		~CSystemClass();

		void Initialise(HINSTANCE _hInstance);
		void Run();

		LRESULT CALLBACK MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);
		HINSTANCE GetAppInstance() const;
		CBackBuffer* GetBackBuffer() const;

		bool GetMouseState() const;
		void SetMouseState(bool _bMouseDown);

		CGame* GetGameInstance() const;

		void SetHostGamePtr(CHostGameMenu* _pHostGame);
		void SetJoinGamePtr(CJoinGameMenu* _pHostGame);
		void SetLobbyMenuPtr(CLobbyMenu* _pLobbyMenu);

		// Singleton Methods
		static CSystemClass* GetInstance();
		static void DestroyInstance();

	protected:

	private:
		// private constructor
		CSystemClass();

		// privately called functions
		void ExecuteFrame();
		void CreateRegisterWindow();
		void Process(float _fCurTime, float _fDelta);

	// Member Variables
	public:

	protected:

	private:
		// Singleton Instance
		static CSystemClass* s_pApplication;

		// system app data
		HWND			m_hWnd;
		bool			m_bMouseDown;
		int				m_iWidth;
		int				m_iHeight;
		int				m_iFPS, m_iCounter;
		float			m_fBase;

		// other data
		LPCWSTR			m_pApplicationName;
		HINSTANCE		m_hInstance;		

		// classes used
		CGame*			m_pGame;
		CTimer*			m_pTimer;	
		CHostGameMenu*	m_pHostMenu;
		CJoinGameMenu*	m_pJoinMenu;
		CLobbyMenu*		m_pLobbyMenu;
};

// Windows callback definition
static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

#endif // _CSYSTEMCLASS_H_