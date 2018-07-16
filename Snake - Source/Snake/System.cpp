//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : System.cpp
// Description : The system class is top class of the program which manages the screen, the game loop and Windows callback function. Contains all other classes.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Self Include:
#include "System.h"

// Local Includes:

// Library Includes:


// Implementation:
CSystemClass::CSystemClass() : m_pTimer(nullptr), m_pGame(nullptr), m_iFPS(0), m_fBase(0.0f), m_iCounter(0), m_bMouseDown(false), m_pHostMenu(nullptr), m_pJoinMenu(nullptr), m_pLobbyMenu(nullptr)
{				
																																 
}

CSystemClass::~CSystemClass()
{
	// Remove the window.
	DestroyWindow(m_hWnd);
	// Remove the application instance.
	UnregisterClass(m_pApplicationName, m_hInstance);

	// Delete used classes
	DeleteOBJ(m_pGame);
	DeleteOBJ(m_pTimer);
}

void CSystemClass::Initialise(HINSTANCE _hInstance)
{
	// basic program settings
	m_pApplicationName = L"Snake!";
	m_hInstance = _hInstance;

	// Initialise the windows api.
	CreateRegisterWindow();

	// Create timer 
	m_pTimer = new CTimer;

	// Create game class
	m_pGame = new CGame;

	if(!m_pGame->Initialise(m_hInstance, m_hWnd, m_iWidth, m_iHeight)) // here // here
	{
		// Error message should be handled inside the initialise
		PostQuitMessage(0);
	}

	m_pHostMenu = m_pGame->GetMenu()->GetHostMenu();
	m_pJoinMenu = m_pGame->GetMenu()->GetJoinMenu();

	// hide cursor
	ShowCursor(true);
}

void CSystemClass::CreateRegisterWindow()
{
	WNDCLASSEX wc;

	// Setup the windows class with default settings.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_pApplicationName;
	wc.cbSize = sizeof(WNDCLASSEX);
	
	// Register the window class.
	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Registering Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}

	// Size
	m_iWidth = SCREENWIDTH;;
	m_iHeight = SCREENHEIGHT;

	// Centre screen
	int iX = (GetSystemMetrics(SM_CXSCREEN) - m_iWidth) / 2;
	int iY = (GetSystemMetrics(SM_CYSCREEN) - m_iHeight) / 2;

	// Create the window with the screen settings and get the handle to it.
	m_hWnd = CreateWindowEx(NULL, m_pApplicationName, m_pApplicationName, 
						WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
						iX, iY, m_iWidth, m_iHeight,
						NULL, NULL, m_hInstance, NULL);

	// check if creating the window failed
	if(!m_hWnd)
	{
		MessageBox(NULL, L"Creating Windows Class Failed", 0, 0);
		PostQuitMessage(0);
	}
}

void CSystemClass::Run()
{
	MSG msg;

	// Initialise the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	while(msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			ExecuteFrame();
		}
	}
}

void CSystemClass::ExecuteFrame()
{
	// tick the timer
	m_pTimer->Process();

	// Process game
	Process(m_pTimer->GetGameTime(), m_pTimer->GetDeltaTick());

	// Render game
	m_pGame->Render();
}

void CSystemClass::Process(float _fCurTime, float _fDelta)
{
	// update fps
	++m_iCounter;

	if((_fCurTime - m_fBase) >= 1.0f )
	{
		m_iFPS = m_iCounter;
		m_iCounter = 0;
		m_fBase = _fCurTime;
	}

	m_pGame->Process(_fDelta);
}

CBackBuffer* CSystemClass::GetBackBuffer() const
{
	return (m_pGame->GetBackBuffer());
}

HINSTANCE CSystemClass::GetAppInstance() const
{
	return(m_hInstance);
}

void CSystemClass::SetMouseState(bool _bMouseDown)
{
	m_bMouseDown = _bMouseDown;
}

bool CSystemClass::GetMouseState() const
{
	return(m_bMouseDown);
}

CSystemClass* CSystemClass::GetInstance()
{
	if (s_pApplication == nullptr)
	{
		s_pApplication = new CSystemClass;
	}

	return (s_pApplication);
}

void CSystemClass::DestroyInstance()
{
	DeleteOBJ(s_pApplication);
}

LRESULT CALLBACK CSystemClass::MessageHandler(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uiMsg)
	{
		// Menu commands
		case WM_COMMAND:
		{
			/*switch(LOWORD(_wParam))
			{
				case ID_FILE_QUIT: EXAMPLE 
				{
					PostQuitMessage(0);
				}
				break;
			}*/
		}
		break;

		case WM_LBUTTONDOWN:
		{
			// if mouse clicked, update bool
			m_bMouseDown = true;
		}
		break;

		case WM_LBUTTONUP:
		{
			// if mouse un-clicked, update bool
			m_bMouseDown = false;
		}
		break;

		case WM_KEYDOWN:
		{
			if(m_pGame->GetState() == MENU)
			{
				m_pHostMenu->GetTextManager()->SetKeyDown(true);
				m_pJoinMenu->GetTextManager()->SetKeyDown(true);

				if(m_pLobbyMenu != nullptr)
				{
					m_pLobbyMenu->GetTextManager()->SetKeyDown(true);
				}
			}
			else
			{
				m_pGame->GetPlay()->SetKeyDown(true);
			}
		}
		break;

		case WM_KEYUP:
		{
			if(m_pGame->GetState() == MENU)
			{
				m_pHostMenu->GetTextManager()->SetKeyDown(false);
				m_pJoinMenu->GetTextManager()->SetKeyDown(false);

				if(m_pLobbyMenu != nullptr)
				{
					m_pLobbyMenu->GetTextManager()->SetKeyDown(false);
				}
			}
			else
			{
				m_pGame->GetPlay()->SetKeyDown(false);
			}
		}
		break;

		default:
		{
			break;
		}
	}
	
	return(DefWindowProc(_hwnd, _uiMsg, _wParam, _lParam));
}

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch(_uiMsg)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return(CSystemClass::GetInstance()->MessageHandler(_hwnd, _uiMsg, _wParam, _lParam));
		}
	}
}

CGame* CSystemClass::GetGameInstance() const
{
	return (m_pGame);
}

void CSystemClass::SetHostGamePtr(CHostGameMenu* _pHostGame)
{
	m_pHostMenu = _pHostGame;
}

void CSystemClass::SetJoinGamePtr(CJoinGameMenu* _pHostGame)
{
	m_pJoinMenu = _pHostGame;
}

void CSystemClass::SetLobbyMenuPtr(CLobbyMenu* _pLobbyMenu)
{
	m_pLobbyMenu = _pLobbyMenu;
}