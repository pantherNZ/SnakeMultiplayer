#ifndef _CSYSTEMCLASS_H_
#define _CSYSTEMCLASS_H_

// Definitions
#define WIN32_LEAN_AND_MEAN

// Library Includes:
#include <windows.h>

// Local Includes:
#include "utility.h"
#include "timer.h"
#include "game.h"

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
		HWND m_hWnd;
		int m_iWidth;
		int m_iHeight;
		int m_iFPS, m_iCounter;
		float m_fBase;

		// other data
		LPCWSTR m_pApplicationName;
		HINSTANCE m_hInstance;		

		// classes used
		CGame* m_pGame;
		CTimer* m_pTimer;
};

// Windows callback definition
static LRESULT CALLBACK WndProc(HWND _hwnd, UINT _uiMsg, WPARAM _wParam, LPARAM _lParam);

#endif // _CSYSTEMCLASS_H_