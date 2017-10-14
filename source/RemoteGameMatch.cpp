#include "RemoteGameMatch.h"
#include "GameEngine.h"
#include <cstdio>

RemoteGameMatch::RemoteGameMatch(Player* localPlayer, const char* address, int port) :
		GameMatch(new DummyPlayer(), localPlayer), waitingForRequest(true), addrInfo(nullptr), sock(0)
{
	//connect to provided address
	char portString[10];
	snprintf(portString, sizeof(portString), "%d", port);
	if (getaddrinfo(address, portString, NULL, &addrInfo) != 0)
	{
		delete black;
		throw ClientException();
	}

	sock = socket(addrInfo->ai_family, SOCK_STREAM, 0); //bind socket
	if (sock == -1)
	{
		freeaddrinfo(addrInfo);
		delete black;

		throw ClientException();
	}

	if (connect(sock, addrInfo->ai_addr, addrInfo->ai_addrlen) != 0) //try to connect
	{
		close(sock);
		freeaddrinfo(addrInfo);
		delete black;
		throw ClientException();
	}

	lastPing = time(nullptr);
	lastPong = time(nullptr);

	freeaddrinfo(addrInfo);

}
RemoteGameMatch::~RemoteGameMatch()
{
	delete black; //created in constructor
}
void RemoteGameMatch::ReceivePackets()
{
	static const int BUFFER_SIZE = Packet::MAX_PACKET_LENGTH;
	char buffer[BUFFER_SIZE];

	while (recv(sock, buffer, BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT) != -1) //read data without removing from queue
	{
		int bytesRead = FormPacket(buffer, BUFFER_SIZE);

		//consume [bytesRead] bytes from buffer
		recv(sock, buffer, bytesRead, 0);
	}

}
int RemoteGameMatch::FormPacket(char* buffer, int length)
{
	if (length < 1)
		return 0;

	int packetID = buffer[0];
	int bytesRead = 0;
	switch (packetID)
	{
		case Packet::PACKET_BOARD_DATA:
			{
			PacketBoardData packet;
			bytesRead += packet.ReadFromBuffer(buffer, length);
			HandlePacket(packet);
			break;
		}
		case Packet::PACKET_GAME_MOVE:
			{
			PacketGameMove packet;
			bytesRead += packet.ReadFromBuffer(buffer, length);
			HandlePacket(packet);
			break;
		}
		case Packet::PACKET_COMMAND:
			{
			PacketCommand packet;
			bytesRead += packet.ReadFromBuffer(buffer, length);
			HandlePacket(packet);
			break;
		}
		default:
			{
			//received wrong type of packet
			GameEngine::GetInstance().ShowMainMenu();
			GameEngine::GetInstance().DisplayPopup("Disconnected");
		}

	}
	return bytesRead;
}
void RemoteGameMatch::HandlePacket(PacketBoardData& packet)
{
	board = packet.GetBoard();
}
void RemoteGameMatch::HandlePacket(PacketGameMove& packet)
{
	board.DoMove(packet.GetMove(), GetTurningPlayerToken());
}
void RemoteGameMatch::HandlePacket(PacketCommand& packet)
{
	auto command = packet.GetCommand();
	switch (command)
	{
		case PacketCommand::COMMAND_NEXT_TURN:
			{
			EndTurn();
			StartTurn();
			break;
		}
		case PacketCommand::COMMAND_MOVE_REFUSED:
			{
			GetTurningPlayer()->RefuseMove();
			break;

		}
		case PacketCommand::COMMAND_REQUEST_MOVE:
			{
			waitingForRequest = false;
			break;
		}
		case PacketCommand::COMMAND_PING:
			{
			lastPong = time(nullptr);
			break;
		}
		default:
			{
			//Unknown packet command
			GameEngine::GetInstance().ShowMainMenu();
			GameEngine::GetInstance().DisplayPopup("Disconnected");
			break;
		}
	}
}
void RemoteGameMatch::Think()
{
	if (endGame)
		return;

	Player* turningPlayer = GetTurningPlayer();

	if (!waitingForRequest && turningPlayer->IsReady()) //only ask white for input
	{
		//Get player's move and check validity
		GameMove move = turningPlayer->GetMove();

		if (!IsMoveValid(move))
		{
			turningPlayer->RefuseMove();
			return;
		}
		else
		{
			board.DoMove(move, GetTurningPlayerToken()); //predict move
			GameEngine::GetInstance().Draw();
			Send(PacketGameMove(move));
			waitingForRequest = true;
			
		}
	}

	time_t currentTime = time(nullptr);
	if (difftime(currentTime, lastPong) > CONNECTION_TIMEOUT)
	{
		GameEngine::GetInstance().ShowMainMenu();
		GameEngine::GetInstance().DisplayPopup("Disconnected");
		return;
	}

	if (difftime(currentTime, lastPing) > 1) //one second delay between pings to avoid flooding the connection
	{
		Send(PacketCommand(PacketCommand::COMMAND_PING));
		lastPing = time(nullptr);
	}

	ReceivePackets();
}
void RemoteGameMatch::Send(const Packet& packet)
{
	const int bufferLength = Packet::MAX_PACKET_LENGTH;
	char buffer[bufferLength];
	int bytesWritten = packet.WriteToBuffer(buffer, bufferLength);

	send(sock, buffer, bytesWritten, 0);
}

//----DummyPlayer
RemoteGameMatch::DummyPlayer::DummyPlayer() :
		Player(true, "Remote Player")
{
}
RemoteGameMatch::DummyPlayer::~DummyPlayer()
{
}
GameMove RemoteGameMatch::DummyPlayer::GetMove() const
{
	return GameMove();
}
void RemoteGameMatch::DummyPlayer::RefuseMove()
{
}
bool RemoteGameMatch::DummyPlayer::IsReady() const
{
	return false;
}
void RemoteGameMatch::DummyPlayer::OnTurnBegin(const GameMatch& match)
{
}
