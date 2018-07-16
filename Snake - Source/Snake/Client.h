//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Client.cpp
// Description : Controls the client side of the networking using Raknet
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__CLIENT_H__)
#define __CLIENT_H__

// Library Includes

// Local Includes
#include "Utility.h"
#include "Lobby.h"
#include "Play.h"

// Constants

class CClient
{
	// Member Functions
	public:
		CClient();
		~CClient();

		void CreateClient();
		void Process();
		void Ping();

		bool ConnectToServer(tServerInfo* _tServerInfo);

		int ServersFound() const;
		tServerInfo* GetServerInfo(UINT _i) const;
		bool SendLobbyInfo();

		void SetLobbyInfo(CStringA _sName, bool _bReady, tColour _colour);
		void ToggleReady();

		void SendDisconnectNotice();
		void SendSnakeInfo(tGameInfo _tGame);
		void SendDeathNotice();
		void SendApple(POINT _poSnakeHeadPos);
		ePlayerColours GetColour() const;

		void SendScore(UINT _uiScore);	
		void SetPlayPtr(CPlay* _pPlay);

	protected:

	private:

	// Member Variables
	public:

	protected:

	private:
		// General client Data
		RakPeerInterface*			m_pPeer;
		Packet*						m_pPacket;
		std::vector<tServerInfo*>	m_pServerInfo;
		int							m_iServersFound;

		// Lobby Data
		CStringA					m_sName;
		bool						m_bReady;
		tColour						m_colour;

		// Pointers to save function calls
		CLobbyMenu*					m_pLobby;
		CPlay*						m_pPlay;
};	

#endif // __CLIENT_H__