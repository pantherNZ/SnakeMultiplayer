#pragma once

#if !defined(__UTILITY_H__)
#define __UTILITY_H__

#pragma comment(lib, "ws2_32.lib")

// Library Includes
//#include "WindowsIncludes.h"
#include "RakNet Source\RakPeerInterface.h"
#include "RakNet Source\MessageIdentifiers.h"
#include "RakNet Source\BitStream.h"
#include "RakNet Source\RakNetTypes.h"
#include "RakNet Source\GetTime.h"
#include <strstream>
#include <Windows.h>
#include <atlstr.h>
#include <vector>
#include <time.h>
//#include <vld.h>

using namespace RakNet;

// Local Includes

// Constants
#define SCREENWIDTH 1024
#define SCREENHEIGHT 750

#define VALIDATE(a) {if (!a) return (false);}
#define DeleteOBJ(x) {if(x) delete x; x = nullptr;}

#define MAX_CLIENTS 16
#define MAX_SERVERS_DISPLAYED 7
#define DEFAULT_SERVER_PORT 60100
#define DEFAULT_CLIENT_PORT 60000

#define BOUNDARYOFFSET 40
#define GRIDSIZE 20

#define RandomFloat() (rand() / (RAND_MAX + 1.0f)) // who wrote this peice of shit amirite?

// Types
typedef enum eGAMESTATE
{
	MENU = 0,
	GAME
} GAMESTATE;

typedef enum eMENUSTATE
{
	MAINMENU = 0,
	JOINMENU = 1,
	HOSTMENU = 2,
	SETTINGSMENU = 3,
	LOBBYMENU = 4
} MENUSTATE;

typedef enum eMENUBUTTON
{
	JOINSERVER = 0,
	HOSTSERVER = 1,
	QUIT = 2,
	HELP = 3,
	MENUBUTTONMAX = 4
} MENUBUTTON;

typedef enum eGameMessages
{
	ID_GAME_MESSAGE = ID_USER_PACKET_ENUM + 1,			// GAME Messages
	ID_LOBBY_MESSAGE = ID_USER_PACKET_ENUM + 2,			// LOBBY Messages
	ID_DISCONNECT_MESSAGE = ID_USER_PACKET_ENUM + 3,	// DISCONNECTION Messages
	ID_STATECHANGE_MESSAGE = ID_USER_PACKET_ENUM + 4,	// STATECHANGE Messages
	ID_DEATH_MESSAGE = ID_USER_PACKET_ENUM + 5,			// DEATH Messages
	ID_FOOD_MESSAGE = ID_USER_PACKET_ENUM + 6,			// FOOD Messages
	ID_SCORE_MESSAGE = ID_USER_PACKET_ENUM + 7,			// Change in SCORE messages
	ID_GAMEOVER_MESSAGE = ID_USER_PACKET_ENUM + 8,		// GAME OVER message
	ID_DISCONNECT_GAME_MESSAGE = ID_USER_PACKET_ENUM + 9
} GAMEMESSAGES;

typedef enum ePlayerColours
{
	WHITE,
	RED,
	BLUE,
	BROWN,
	YELLOW,
	LIME,
	AQUA,
	TEAL,
	PURPLE,
	PINK,
	ORANGE,
	GREEN,
	SILVER,
	SALMON,
	ROYALBLUE,
	KHAKI
} PLAYERCOLOURS;

typedef enum eSnakeDirections
{
	UP = 1,
	RIGHT = 2,
	DOWN = -1,
	LEFT = -2,
	NONE = 3
} SNAKEDIRECTION;

struct tColour
{
	int iR;
	int iG;
	int iB;
	PLAYERCOLOURS eColourName;

	tColour(int _iR, int _iG, int _iB, PLAYERCOLOURS _eColourName) : iR(_iR), iG(_iG), iB(_iB), eColourName(_eColourName)
	{

	}

	tColour()
	{

	}
};

// FIX these structs used for too many things; split them out
struct tServerInfo
{
	CStringA sServerName;
	CStringA sPlayerName;
	UINT sPort; // Redundant: included in the system address struct
	SystemAddress sAddress;
};

struct tServerPong
{
	CStringA sServerName;
	bool bInGame;
};

struct tPointF
{
	tPointF(float _fX, float _fY, SNAKEDIRECTION _eDirection, int _iTurningPointNumber): fX(_fX), fY(_fY), eDirection(_eDirection), iTurningPointNumber(_iTurningPointNumber)
	{

	}

	tPointF(float _fX, float _fY): fX(_fX), fY(_fY)
	{

	}

	tPointF()
	{

	}

	float fX;
	float fY;
	SNAKEDIRECTION eDirection;
	int iTurningPointNumber;
};

struct tLobbyClientInfo
{
	CStringA sPlayerName;
	// Some form of colour
	bool bReady;
	SystemAddress sAddress;
	tColour colour;
	UINT uiScore;
};

struct tGameInfo
{
	bool bAlive;
	PLAYERCOLOURS ePlayerColour;
	std::vector<tPointF> tSnakePos;
	std::vector<tPointF> tTurningPoints;
};

// Prototypes

// Global inline functions
template<typename T> std::string ToString(const T& _value)
{
	std::strstream theStream;
	theStream << _value << std::ends;

	return (theStream.str());
}

inline bool CheckMenuHighlight(POINT _mousePos, POINT _buttonPos, POINT _buttonOffset)
{
	bool bCollisionX = (_mousePos.x > (_buttonPos.x - _buttonOffset.x)) && (_mousePos.x < (_buttonPos.x + _buttonOffset.x));
	bool bCollisionY = (_mousePos.y > (_buttonPos.y - _buttonOffset.y)) && (_mousePos.y < (_buttonPos.y + _buttonOffset.y));

	if (bCollisionX && bCollisionY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

#endif // __UTILITY_H__