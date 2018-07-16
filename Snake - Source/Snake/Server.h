//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Server.h
// Description : This class manages the server used in the networking via Raknet.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

#pragma once

#if !defined(__SERVER_H__)
#define __SERVER_H__

// Library Includes

// Local Includes
#include "Utility.h"

// Constants

class CServer
{
	// Member Functions
	public:
		CServer();
		~CServer();

		void CreateServer(tServerInfo _tServerInfo);
		void Process();

		void SendOutLobbyInfo();
		void SendOutDisconnectInfo(CStringA _sName);
		void SendOutDisconnectInfoGame(PLAYERCOLOURS _ePlayerColour);

		void CheckGameStart();
		void StateChangeMessage(GAMESTATE _eGameState);

		void GameStart();
		void SendOutGameInfo();
		void SendOutDeathInfo(UINT _i);
		void SendOutFoodInfo();
		void SendOutHighScore();

	protected:

	private:		
		void CheckGameOver();
		void GrowApples();

	// Member Variables
	public:

	protected:

	private:
		RakPeerInterface*	m_pPeer;
		Packet*				m_pPacket;

		std::vector<tLobbyClientInfo*>	m_vClientsLobbyInfo;
		std::vector<tGameInfo*>			m_vClientsGameInfo;
		std::vector<tColour>			m_crAvailableColours;
		std::vector<POINT>				m_vApplePositions;

		GAMESTATE						m_eGameState;
		UINT							m_uiHighScore;
		CStringA						m_strLeader;
		CStringA						m_strServerName;
};

#endif // __SERVER_H__