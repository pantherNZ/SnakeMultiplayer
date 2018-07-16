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

// Library Includes

// Local Includes

// This include
#include "Client.h"
#include "system.h"

// Static Variables

// Static Function Prototypes

// Implementation
CClient::CClient() : m_pPeer(nullptr), m_pPacket(nullptr), m_iServersFound(0), m_colour(255, 255, 255, WHITE), m_bReady(false), m_sName(""), m_pPlay(nullptr), m_pLobby(nullptr)
{

}

CClient::~CClient()
{
	for (UINT i = 0; i < m_pServerInfo.size(); ++i)
	{
		DeleteOBJ(m_pServerInfo[i]);
	}

	m_pServerInfo.clear();

	m_pPeer->DeallocatePacket(m_pPacket);
	m_pPeer->Shutdown(300);
	RakPeerInterface::DestroyInstance(m_pPeer);
}

void CClient::CreateClient()
{
	m_pPeer = RakPeerInterface::GetInstance();

	SocketDescriptor rSocketDescriptor;
	m_pPeer->Startup(1, &rSocketDescriptor, 1);

	// We ping all of the ports in the server range to try find active servers
	Ping();
}

bool CClient::ConnectToServer(tServerInfo* _tServerInfo)
{	
	tServerInfo* pServerInfo = new tServerInfo;
	pServerInfo->sAddress = _tServerInfo->sAddress;
	pServerInfo->sPlayerName = _tServerInfo->sPlayerName;
	pServerInfo->sPort = _tServerInfo->sPort;
	pServerInfo->sServerName = _tServerInfo->sServerName;

	m_pPeer->Connect(pServerInfo->sAddress.ToString(), pServerInfo->sPort, 0, 0);

	// Clear the server vector so it only holds the server we have connected to.
	for (UINT i = 0; i < m_pServerInfo.size(); ++i)
	{
		DeleteOBJ(m_pServerInfo[i]);
	}

	m_pServerInfo.clear();

	m_pServerInfo.push_back(pServerInfo);

	return (true);
}

void CClient::Ping()
{
	m_pServerInfo.clear();
	m_iServersFound = 0;

	// We ping all of the ports that can be used, and we specify we only want a return 'pong' from servers that have connection spots remaining
	for (int i = DEFAULT_SERVER_PORT; i < DEFAULT_SERVER_PORT + 100; ++i)
	{
		m_pPeer->Ping("255.255.255.255", i, true);
	}
}

void CClient::Process()
{
	// For each packet
	for (m_pPacket = m_pPeer->Receive(); m_pPacket; m_pPeer->DeallocatePacket(m_pPacket), m_pPacket = m_pPeer->Receive())
	{
		switch (m_pPacket->data[0])
		{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			{
				MessageBox(NULL, L"ID_REMOTE_DISCONNECTION_NOTIFICATION", L"ERROR", MB_OK);
			}
			break;

			case ID_REMOTE_CONNECTION_LOST:
			{
				MessageBox(NULL, L"ID_REMOTE_CONNECTION_LOST", L"ERROR", MB_OK);
			}
			break;

			case ID_REMOTE_NEW_INCOMING_CONNECTION:
			{
				MessageBox(NULL, L"ID_REMOTE_NEW_INCOMING_CONNECTION", L"ERROR", MB_OK);
			}
			break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
			{
				SendLobbyInfo();
			}
			break;	

			case ID_NEW_INCOMING_CONNECTION:
			{
				MessageBox(NULL, L"ID_NEW_INCOMING_CONNECTION", L"ERROR", MB_OK);
			}
			break;

			case ID_NO_FREE_INCOMING_CONNECTIONS:
			{
				MessageBox(NULL, L"ID_NO_FREE_INCOMING_CONNECTIONS", L"ERROR", MB_OK);
			}
			break;

			case ID_DISCONNECTION_NOTIFICATION:
			{
				// If we have lost connection to the server
				MessageBox(NULL, L"The server is no longer available", L"ERROR", MB_OK);
				CSystemClass::GetInstance()->GetGameInstance()->SetState(MENU);
				return;
			}
			break;

			case ID_CONNECTION_LOST:
			{
				// If we have lost connection
				MessageBox(NULL, L"The connection to the server has been lost.", L"ERROR", MB_OK);
				PostQuitMessage(0);
			}
			break;

			case ID_UNCONNECTED_PONG: // A server has responded to our 'ping'
			{
				TimeMS time;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false);
				bsIn.IgnoreBytes(1);
				bsIn.Read(time);

				unsigned int dataLength = m_pPacket->length - sizeof(unsigned char) - sizeof(RakNet::TimeMS);

				if (dataLength > 0)
				{
					CStringW sServerName = m_pPacket->data + sizeof(unsigned char) + sizeof(RakNet::TimeMS);
					sServerName.Delete(dataLength, (sServerName.GetLength() - dataLength));

					// We have found a server and recieved its name. Send that name to joingamemenu so it can display it
					tServerInfo* pNewServer = new tServerInfo;

					pNewServer->sServerName = sServerName;
					pNewServer->sAddress = m_pPacket->systemAddress;
					pNewServer->sPort = m_pPacket->systemAddress.GetPort();

					bool bPush = true;

					for (UINT i = 0; i < m_pServerInfo.size(); ++i) // If we remove checking the address on the line below servers cannot have the same name as another, only the first one found would be shown.
					{
						if (pNewServer->sServerName == m_pServerInfo[i]->sServerName && pNewServer->sAddress == m_pServerInfo[i]->sAddress)
						{
							bPush = false;
						}
					}

					if (bPush)
					{
						m_pServerInfo.push_back(pNewServer);
						m_iServersFound++;
					} 
					else
					{
						DeleteOBJ(pNewServer);
					}							
				}
			}
			break;

			case ID_LOBBY_MESSAGE:
			{
				if(m_pLobby == nullptr)
				{
					m_pLobby = CSystemClass::GetInstance()->GetGameInstance()->GetMenu()->GetLobbyMenu();
				}

				// Recieved a packet from the server with lobby information
				RakString rsName;
				bool bReady;
				tColour colour;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(rsName);
				bsIn.Read(bReady);
				bsIn.Read(colour);
				
				if(rsName.C_String() == m_sName)
				{
					m_colour = colour;
				}

				// Add user to the client list
				m_pLobby->AddToClientList(rsName.C_String(), bReady, colour);
			}
			break;

			case ID_DISCONNECT_MESSAGE:
			{
				if(m_pLobby == nullptr)
				{
					m_pLobby = CSystemClass::GetInstance()->GetGameInstance()->GetMenu()->GetLobbyMenu();
				}

				RakString rsName;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(rsName);

				if (CSystemClass::GetInstance()->GetGameInstance()->GetState() == MENU)
				{
					// Remove user
					m_pLobby->RemoveFromClientList(rsName.C_String());
				}
			}
			break;

			// If someone has left the game
			case ID_DISCONNECT_GAME_MESSAGE:
			{
				PLAYERCOLOURS _colour;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(_colour);

				m_pPlay->DisconnectionMessageFromUserLeavingTheGameDuringGameInProgess(_colour);
			}
			break;

			case ID_STATECHANGE_MESSAGE: // Packet containing an updated state
			{
				GAMESTATE eGameState;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(eGameState);

				// Change state(MENU -> GAME, GAME -> MENU)
				CSystemClass::GetInstance()->GetGameInstance()->SetState(eGameState);
			}
			break;

			// Main game loop message
			case ID_GAME_MESSAGE:
			{
				if(m_pPlay == nullptr)
				{
					m_pPlay = CSystemClass::GetInstance()->GetGameInstance()->GetPlay();
				}

				// Temp data for reading in
				tGameInfo tGame;
				UINT uiSizePos, uiSizeTurning;
				std::vector<tPointF> vecTurningPoints, vecPositions;
				tPointF newPointPos, newPointTurning;

				// read in information
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(tGame.ePlayerColour);
				bsIn.Read(uiSizePos);
				bsIn.Read(uiSizeTurning);

				for(UINT i = 0; i <  uiSizePos; ++i)
				{			
					bsIn.Read(newPointPos);
					vecPositions.push_back(newPointPos);
				}

				for(UINT i = 0; i <  uiSizeTurning; ++i)
				{			
					bsIn.Read(newPointTurning);
					vecTurningPoints.push_back(newPointTurning);
				}
				
				// create struct to pass to play class
				bool bPlayer = (tGame.ePlayerColour == m_colour.eColourName);
				tGame.bAlive = true;
				tGame.tSnakePos = vecPositions;
				tGame.tTurningPoints = vecTurningPoints;

				if(m_pPlay != nullptr)
				{
					// store information
					m_pPlay->AddToSnakeList(tGame, bPlayer);
				}
			}
			break;

			case ID_DEATH_MESSAGE:
			{
				// Someone has died // BUT WHOOOOOOOOOOOOOOO
				PLAYERCOLOURS ePlayerColour;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID)); // Skip the message ID
				bsIn.Read(ePlayerColour);

				m_pPlay->DeathNotice(ePlayerColour);
			}
			break;

			// message for updating food positions
			case ID_FOOD_MESSAGE:
			{
				if(m_pPlay == nullptr)
				{
					m_pPlay = CSystemClass::GetInstance()->GetGameInstance()->GetPlay();
				}

				UINT iLength;
				std::vector<POINT> ApplePos;
				POINT poNewPoint;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(iLength);

				for (UINT i = 0; i < iLength; ++i)
				{
					bsIn.Read(poNewPoint);
					ApplePos.push_back(poNewPoint);
				}

				// add apple to vector in play
				m_pPlay->AddToAppleList(ApplePos);
			}
			break;

			// message for updating highest score
			case ID_SCORE_MESSAGE:
			{
				UINT uiHighScore;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(uiHighScore);

				m_pPlay->SetHighScore(uiHighScore);
			}
			break;

			// message for if the game has ended
			case ID_GAMEOVER_MESSAGE:
			{
				RakString rsString;
				BitStream bsIn(m_pPacket->data, m_pPacket->length, false); // Create bitstream from packet data
				bsIn.IgnoreBytes(sizeof(MessageID));
				bsIn.Read(rsString);

				// call gameover function
				m_pPlay->GameOver(rsString.C_String(), (rsString.C_String() == m_sName));
			}
			break;

			default: break;
		}
	}
}

int CClient::ServersFound() const
{
	return (m_iServersFound);
}

tServerInfo* CClient::GetServerInfo(UINT _i) const
{
	return (m_pServerInfo[_i]);
}

void CClient::SetLobbyInfo(CStringA _sName, bool _bReady, tColour _colour)
{
	m_sName = _sName;
	m_bReady = _bReady;
	m_colour = _colour;
}

void CClient::ToggleReady()
{
	m_bReady = !m_bReady;
}

void CClient::SendDisconnectNotice()
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_DISCONNECTION_NOTIFICATION);
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);
}

bool CClient::SendLobbyInfo()
{
	RakString rsName = m_sName;

	BitStream bsOut;
	bsOut.Write((MessageID) ID_LOBBY_MESSAGE);
	bsOut.Write(rsName);
	bsOut.Write(m_bReady);
	bsOut.Write(m_colour);

	// send out our lobby information to the server
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);

	return (true);
}

void CClient::SendSnakeInfo(tGameInfo _tGame)
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_GAME_MESSAGE);
	_tGame.ePlayerColour = m_colour.eColourName;
	
	// write information to the bit stream for output
	bsOut.Write(_tGame.ePlayerColour);
	bsOut.Write(_tGame.tSnakePos.size());
	bsOut.Write(_tGame.tTurningPoints.size());

	for (UINT i = 0; i < _tGame.tSnakePos.size(); ++i)
	{
		bsOut.Write(_tGame.tSnakePos[i]);
	}

	for (UINT i = 0; i < _tGame.tTurningPoints.size(); ++i)
	{
		bsOut.Write(_tGame.tTurningPoints[i]);
	}

	// send out our snake information to the server
	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);
}

void CClient::SendDeathNotice()
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_DEATH_MESSAGE);

	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);
}

ePlayerColours CClient::GetColour() const
{
	return (m_colour.eColourName);
}

void CClient::SendApple(POINT _poSnakeHeadPos)
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_FOOD_MESSAGE);
	bsOut.Write(_poSnakeHeadPos);

	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);
}

void CClient::SendScore(UINT _uiScore)
{
	BitStream bsOut;
	bsOut.Write((MessageID) ID_SCORE_MESSAGE);
	bsOut.Write(_uiScore);

	m_pPeer->Send(&bsOut, HIGH_PRIORITY, RELIABLE, 0, m_pServerInfo[0]->sAddress, false);
}

void CClient::SetPlayPtr(CPlay* _pPlay)
{
	m_pPlay = _pPlay;
}