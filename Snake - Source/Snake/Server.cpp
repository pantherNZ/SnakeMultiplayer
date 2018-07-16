//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// 2014 (C) Media Design School
//
// File Name : Server.cpp
// Description : This class manages the server used in the networking via Raknet.
// Author : Alex Denford, Michael Green
// Mail : alexander.denford@mediadesign.school.nz, michael.green@mediadesign.school.nz
//

// Library Includes

// Local Includes
#include "system.h"

// This include
#include "Server.h"

// Static Variables

// Static Function Prototypes

// Implementation
CServer::CServer() : m_pPeer(nullptr), m_pPacket(nullptr), m_eGameState(MENU), m_uiHighScore(0), m_strLeader("")
{
	// Available player colours:
	m_crAvailableColours.push_back(tColour(255, 255, 255, WHITE));		// White	
	m_crAvailableColours.push_back(tColour(255,   0,   0, RED));		// Red
	m_crAvailableColours.push_back(tColour( 72, 118, 255, BLUE));		// Blue
	m_crAvailableColours.push_back(tColour(255, 153,  51, BROWN));		// Brown
	m_crAvailableColours.push_back(tColour(255, 255,   0, YELLOW));		// Yellow
	m_crAvailableColours.push_back(tColour(142, 255,   0, LIME));		// Lime
	m_crAvailableColours.push_back(tColour(  0, 245, 255, AQUA));		// Aqua
	m_crAvailableColours.push_back(tColour(  0, 255, 127, TEAL));		// Teal
	m_crAvailableColours.push_back(tColour(155,  48, 255, PURPLE));		// Purple
	m_crAvailableColours.push_back(tColour(238,  18, 137, PINK));		// Pink
	m_crAvailableColours.push_back(tColour(255, 165,   0, ORANGE));		// Orange
	m_crAvailableColours.push_back(tColour(	 0, 204,   0, GREEN));		// Green
	m_crAvailableColours.push_back(tColour(192, 192, 192, SILVER));		// Silver
	m_crAvailableColours.push_back(tColour(250, 128, 114, SALMON));		// Salmon
	m_crAvailableColours.push_back(tColour(125, 158, 192, ROYALBLUE));	// Royal Blue
	m_crAvailableColours.push_back(tColour(240, 230, 140, KHAKI));		// Khaki
}

CServer::~CServer()
{	
	m_pPeer->DeallocatePacket(m_pPacket);
	m_pPeer->Shutdown(300);
	RakPeerInterface::DestroyInstance(m_pPeer);

	for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
	{
		DeleteOBJ(m_vClientsLobbyInfo[i]);
	}

	m_vClientsLobbyInfo.clear();

	for (UINT i = 0; i < m_vClientsGameInfo.size(); ++i)
	{
		DeleteOBJ(m_vClientsGameInfo[i]);
	}

	m_vClientsGameInfo.clear();
}

void CServer::CreateServer(tServerInfo _tServerInfo)
{
	m_pPeer = RakNet::RakPeerInterface::GetInstance();

	SocketDescriptor rSocketDescriptor(_tServerInfo.sPort, 0);

	m_pPeer->Startup(MAX_CLIENTS, &rSocketDescriptor, 1);
	m_pPeer->SetMaximumIncomingConnections(MAX_CLIENTS);

	//m_strServerName = _tServerInfo.sServerName;

	//CStringA str = m_strServerName;
	//str.Append("FALSE");
	//const char* pChar = (const char*)str;

	// We need to make our 'pong' send the name of our server
	m_pPeer->SetOfflinePingResponse(_tServerInfo.sServerName, sizeof(_tServerInfo.sServerName));
	//m_pPeer->SetOfflinePingResponse(pChar, sizeof(&pChar) + 1);
}

void CServer::Process()
{
 	switch (m_eGameState)
	{
		case (MENU):
		{
			if (m_vClientsLobbyInfo.size() > 0)
			{
				SendOutLobbyInfo();
				CheckGameStart();
			}
		}
		break;

		case (GAME):
		{
			SendOutGameInfo();

			GrowApples();
		}
		break;

		default: break;
	};

	for (m_pPacket = m_pPeer->Receive(); m_pPacket; m_pPeer->DeallocatePacket(m_pPacket), m_pPacket = m_pPeer->Receive())
	{
		switch (m_pPacket->data[0])
		{
			case ID_NEW_INCOMING_CONNECTION: // A Client has wants to connect to our server, add them to our list of clients for easy access
			{
				if(m_eGameState != GAME)
				{
					// Create a dummy client info struct and add it to our connected clients
					tLobbyClientInfo* tNewClient = new tLobbyClientInfo;
					tNewClient->bReady = false;
					tNewClient->sAddress = m_pPacket->systemAddress;
					tNewClient->sPlayerName = "@NAMENOTSET@"; // This will be updated when the new client updates the server
					tNewClient->uiScore = 0;
				
					// Assign a random colour from all available colours
					int iColourIndex = rand() % m_crAvailableColours.size();
					tNewClient->colour = m_crAvailableColours[iColourIndex];
					m_crAvailableColours.erase(m_crAvailableColours.begin() + iColourIndex);

					// Add to the vector
					m_vClientsLobbyInfo.push_back(tNewClient);
				}
			}
			break;

			case ID_DISCONNECTION_NOTIFICATION:
			{
				// We need to iterate through all of our clients to find out who disconnected
				int iDisconnectedClient = 0;

				for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
				{
					if (m_vClientsLobbyInfo[i]->sAddress == m_pPacket->systemAddress)
					{
						iDisconnectedClient = i;
						break;
					}
				}

				// Add colour back into available colours
				m_crAvailableColours.push_back(m_vClientsLobbyInfo[iDisconnectedClient]->colour);

				if (m_eGameState == MENU)
				{
					// Send out disconnect info
					SendOutDisconnectInfo(m_vClientsLobbyInfo[iDisconnectedClient]->sPlayerName);
				}
				else if (m_eGameState == GAME)
				{
					// Send out disconnect info
				//	SendOutDisconnectInfo(m_vClientsLobbyInfo[iDisconnectedClient]->sPlayerName);
				}			

				// Now remove the client information from our list of connected clients
				DeleteOBJ(m_vClientsLobbyInfo[iDisconnectedClient]);


				m_vClientsLobbyInfo.erase(m_vClientsLobbyInfo.begin() + iDisconnectedClient);

				if (m_eGameState == GAME)
				{
					m_vClientsGameInfo.erase(m_vClientsGameInfo.begin() + iDisconnectedClient);
					CheckGameOver();
				}
			}
			break;

			case ID_LOBBY_MESSAGE: // Packet with lobby information
			{
				// Recieve packet from client in form of bitstream
				RakString rsName;
				bool bReady;
				tColour crColour;

				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(rsName);
				bsIn.Read(bReady);
				bsIn.Read(crColour);

				// Find the client who sent us data and update their contents
				for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
				{
					if (m_vClientsLobbyInfo[i]->sAddress == m_pPacket->systemAddress)
					{
						m_vClientsLobbyInfo[i]->bReady = bReady;
						m_vClientsLobbyInfo[i]->sPlayerName = rsName.C_String();

						if(crColour.iR == 0 && crColour.iG == 0 && crColour.iB == 0)
						{
							m_vClientsLobbyInfo[i]->colour = crColour;
						}
					}
				}
			}
			break; 
			
			case ID_GAME_MESSAGE: // Packet with game information
			{
				// Recieve packet from client in form of bitstream
				std::vector<tPointF> vecTurningPoints, vecPositions;
				tPointF tempPointPos, tempPointTurning;
				PLAYERCOLOURS ePlayerColour;
				UINT iLengthPos, iLengthTurning;

				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(ePlayerColour);
				bsIn.Read(iLengthPos);
				bsIn.Read(iLengthTurning);

				for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
				{
					if (m_vClientsLobbyInfo[i]->sAddress == m_pPacket->systemAddress)
					{
						m_vClientsGameInfo[i]->ePlayerColour = ePlayerColour;

						for (UINT j = 0; j < iLengthPos; ++j)
						{
							bsIn.Read(tempPointPos);	

							vecPositions.push_back(tempPointPos);
						}	

						for (UINT j = 0; j < iLengthTurning; ++j)
						{
							bsIn.Read(tempPointTurning);

							vecTurningPoints.push_back(tempPointTurning);
						}

						m_vClientsGameInfo[i]->tSnakePos = vecPositions;
						m_vClientsGameInfo[i]->tTurningPoints = vecTurningPoints;

						break;
					}
				}
			}
			break;

			case ID_DEATH_MESSAGE:
			{
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false);
				
				// Find the client who sent us data and update kill them
				for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
				{
					if (m_vClientsLobbyInfo[i]->sAddress == m_pPacket->systemAddress)
					{
						m_vClientsGameInfo[i]->bAlive = false;

						// Tell the clients someone has died

						SendOutDeathInfo(i);

						break;
					}
				}

				CheckGameOver();
			}
			break;

			case ID_FOOD_MESSAGE:
			{
				POINT p;

				BitStream bsIn(m_pPacket->data, m_pPacket->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(p);

				float fDistance = 99999.0f;
				int iClosestIndex = 0;

				// Search through the apples and find the closest one
				for(UINT i = 0; i < m_vApplePositions.size(); ++i)
				{
					float fX = float(m_vApplePositions[i].x - p.x);
					float fY = float(m_vApplePositions[i].y - p.y);
					float fNewDistance = sqrt((fX * fX) + (fY * fY));

					if(fNewDistance < fDistance)
					{
						fDistance = fNewDistance;
						iClosestIndex = i;
					}
				}

				m_vApplePositions.erase(m_vApplePositions.begin() + iClosestIndex);

				// Update everyones food info (sending out a broadcasted packet with new info)
				SendOutFoodInfo();
			}
			break;

			case ID_SCORE_MESSAGE:
			{
				UINT uiScore;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(uiScore);
				
				for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
				{
					if (m_vClientsLobbyInfo[i]->sAddress == m_pPacket->systemAddress)
					{
						m_vClientsLobbyInfo[i]->uiScore = uiScore;

						if (uiScore > m_uiHighScore)
						{
							m_uiHighScore = uiScore;
							m_strLeader = m_vClientsLobbyInfo[i]->sPlayerName;

							// Send out the new highscore
							SendOutHighScore();
							CheckGameOver();
						}

						break;
					}
				}
			}
			break;
			
			default: break;
		}
	}
}

void CServer::SendOutHighScore()
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_SCORE_MESSAGE);
	bsOut.Write(m_uiHighScore);

	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}

void CServer::GrowApples()
{
	float fNumber = float(m_vClientsGameInfo.size() / 2.0f) + 0.5f;
	UINT uiApples = (UINT)floor(fNumber);

	// If there are less than the # of apples we want
	if(m_vApplePositions.size() < uiApples)
	{
		// depending on the number that are missing, add new apples!
		for(UINT i = m_vApplePositions.size(); i < uiApples; ++i)
		{
			// random position alligned with the grid
			int iMaxGridLinesWidth = (SCREENWIDTH - BOUNDARYOFFSET * 4) / GRIDSIZE;
			int iMaxGridLinesHeight = (SCREENHEIGHT - BOUNDARYOFFSET * 4) / GRIDSIZE;
			int iX = BOUNDARYOFFSET * 2 + GRIDSIZE * (rand() % iMaxGridLinesWidth);
			int iY = BOUNDARYOFFSET * 2 + GRIDSIZE * (rand() % iMaxGridLinesHeight);
			POINT p = {iX, iY};
			m_vApplePositions.push_back(p);
		}

		// Update everyones food info (sending out a broadcasted packet with new info)
		SendOutFoodInfo();
	}
}

void CServer::SendOutFoodInfo()
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_FOOD_MESSAGE);
	bsOut.Write(m_vApplePositions.size());

	for(UINT i = 0; i < m_vApplePositions.size(); ++i)
	{
		bsOut.Write(m_vApplePositions[i]);
	}
			
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}

void CServer::SendOutLobbyInfo()
{
	// Broadcast the client information to connected clients
	for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
	{
		RakString rsName = m_vClientsLobbyInfo[i]->sPlayerName;
		
		BitStream bsOut;
		bsOut.Write((MessageID) ID_LOBBY_MESSAGE);
		bsOut.Write(rsName);
		bsOut.Write(m_vClientsLobbyInfo[i]->bReady);
		bsOut.Write(m_vClientsLobbyInfo[i]->colour);
		
		m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);

	}
}

void CServer::SendOutGameInfo()
{
	// Broadcast the client information to connected clients
	for (UINT i = 0; i < m_vClientsGameInfo.size(); ++i)
	{	
		// Only send out info if the player is alive
		if (m_vClientsGameInfo[i]->bAlive)
		{
			BitStream bsOut;
			bsOut.Write((MessageID) ID_GAME_MESSAGE);

			bsOut.Write(m_vClientsGameInfo[i]->ePlayerColour);
			bsOut.Write(m_vClientsGameInfo[i]->tSnakePos.size());
			bsOut.Write(m_vClientsGameInfo[i]->tTurningPoints.size());

			for (UINT j = 0; j < m_vClientsGameInfo[i]->tSnakePos.size(); ++j)
			{
				bsOut.Write(m_vClientsGameInfo[i]->tSnakePos[j]);
			}

			for (UINT j = 0; j < m_vClientsGameInfo[i]->tTurningPoints.size(); ++j)
			{
				bsOut.Write(m_vClientsGameInfo[i]->tTurningPoints[j]);
			}
			
			m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
		}
	}
}

void CServer::GameStart()
{
	tPointF startPositions[16];

	for(int i = 0; i < 16; ++i)
	{
		startPositions[i].fX = float((i % 2 == 0 ? BOUNDARYOFFSET + GRIDSIZE * 4 : SCREENWIDTH - BOUNDARYOFFSET - GRIDSIZE * 4));
		startPositions[i].fY = float(BOUNDARYOFFSET + GRIDSIZE * 4  + (i - (i % 2)) * GRIDSIZE * 2);
		startPositions[i].eDirection = (i % 2 == 0 ? RIGHT : LEFT);
		startPositions[i].iTurningPointNumber = -1;
	}

	for(UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
	{
		tGameInfo* tempClient = new tGameInfo;

		tempClient->bAlive = true;
		tempClient->ePlayerColour = m_vClientsLobbyInfo[i]->colour.eColourName;
		tempClient->tSnakePos.push_back(startPositions[i]);

		m_vClientsGameInfo.push_back(tempClient);
	}
}

void CServer::SendOutDisconnectInfo(CStringA _sName)
{
	RakString rsName = _sName;
		
	BitStream bsOut;
	bsOut.Write((MessageID) ID_DISCONNECT_MESSAGE);
	bsOut.Write(rsName);
		
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}

void CServer::SendOutDisconnectInfoGame(PLAYERCOLOURS _ePlayerColour)
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_DISCONNECT_GAME_MESSAGE);
	bsOut.Write(_ePlayerColour);
		
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}

void CServer::SendOutDeathInfo(UINT _i)
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_DEATH_MESSAGE);
	bsOut.Write(m_vClientsGameInfo[_i]->ePlayerColour);
		
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}

void CServer::CheckGameStart()
{
	bool bGameStart = true;

	for (UINT i = 0; i < m_vClientsLobbyInfo.size(); ++i)
	{
		if (!(m_vClientsLobbyInfo[i]->bReady))
		{
			bGameStart = false;
			break;
		}
	}

	if (bGameStart && m_vClientsLobbyInfo.size() > 1)
	{
		// All of the connected clients (2 - 16) are ready to start the game!
		// Send a message to all clients notifying them to change their game state to GAME
		StateChangeMessage(GAME);
	}
}

void CServer::CheckGameOver()
{
	int iAlive = 0;
	UINT uiLastPersonScore = 0;

	for (UINT i = 0; i < m_vClientsGameInfo.size(); ++i)
	{
		if (m_vClientsGameInfo[i]->bAlive)
		{
			++iAlive;
			uiLastPersonScore = m_vClientsLobbyInfo[i]->uiScore;
		}
	}

	// Only end if 1 person is left and there score is the highest score OR if there is no-one alive
	if (iAlive < 1 || (uiLastPersonScore >= m_uiHighScore && iAlive == 1))
	{
		RakString rsName = m_strLeader;

		BitStream bsOut; 
		bsOut.Write((MessageID) ID_GAMEOVER_MESSAGE);
		bsOut.Write(rsName);
		
		m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
	}
}

void CServer::StateChangeMessage(GAMESTATE _eGameState)
{			
	m_eGameState = _eGameState;

	BitStream bsOut;
	bsOut.Write((MessageID) ID_STATECHANGE_MESSAGE);
	bsOut.Write(_eGameState);
		
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_RAKNET_GUID, true);
}