// Library Includes:

// Local Includes:
#include "utility.h"
#include "System.h"

// Initialise pointer of instance
CSystemClass* CSystemClass::s_pApplication = nullptr;

// winmain
int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iShowCmd)
{
	srand((unsigned int)(time(0)));

	CSystemClass* pApp = CSystemClass::GetInstance();

	pApp->Initialise(_hInstance);

	pApp->Run();

	CSystemClass::DestroyInstance();

	return(0);
}
/*
#define LEAKS 99999

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, PSTR _cmdLine, int _iShowCmd)
{
	for(int i = 0; i < LEAKS; ++i)
	{
		CSystemClass* pApp = new CSystemClass;
	}

	sleep(100000);

	return(0);
}
*/