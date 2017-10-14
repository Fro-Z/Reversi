#include "RemotePlayer.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include "Network/PacketCommand.h"
#include "Network/PacketGameMove.h"
#include "GameMatch.h"
#include "GameEngine.h"

RemotePlayer::RemotePlayer(const std::string& name) :
		Player(true, name), addrInfo(nullptr), ready(false), waitingForMove(false), sock(0), connDescriptor(0)
{
	char portStr[10];
	snprintf(portStr, 10, "%d", LISTEN_PORT);

	if (getaddrinfo("localhost", portStr, NULL, &addrInfo) != 0)
		throw ServerException();

	sock = socket(addrInfo->ai_family, SOCK_STREAM, 0); //default is TCP for SOCK_STREAM
	if (sock == -1) //error creating socket
	{
		freeaddrinfo(addrInfo); //destructor would not be called
		throw ServerException();
	}

	int value = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value)); //allows immediate reuse of the socket after match ends

	if (bind(sock, addrInfo->ai_addr, addrInfo->ai_addrlen) != 0) //error binding socket
	{
		close(sock);
		freeaddrinfo(addrInfo);
		throw ServerException();
	}
	if (listen(sock, 5) != 0) //start listening to connections
	{
		close(sock);
		freeaddrinfo(addrInfo);
		throw ServerException();
	}

	GameEngine::GetInstance().DisplayPopup("Waiting for connection");
	GameEngine::GetInstance().Draw();

	connDescriptor = accept(sock, addrInfo->ai_addr, &addrInfo->ai_addrlen); //try to accept connection, blocks
	if (connDescriptor == -1)
	{
		close(sock);
		freeaddrinfo(addrInfo);
		throw ServerException();
	}

	lastPong = time(nullptr);
	lastPing = time(nullptr);

	freeaddrinfo(addrInfo);
}
RemotePlayer::~RemotePlayer()
{
	close(connDescriptor);
	close(sock);
}
bool RemotePlayer::IsReady() const
{
	return ready;
}
GameMove RemotePlayer::GetMove() const
{
	return move;
}
void RemotePlayer::RefuseMove()
{
	ready = false;
	Send(PacketCommand(PacketCommand::COMMAND_MOVE_REFUSED));
	Send(PacketCommand(PacketCommand::COMMAND_REQUEST_MOVE));
}
void RemotePlayer::OnTurnBegin(const GameMatch& match)
{
	Send(PacketCommand(PacketCommand::COMMAND_REQUEST_MOVE));
	ready = false;
}
void RemotePlayer::ReceivePackets()
{
	static const int BUFFER_SIZE = Packet::MAX_PACKET_LENGTH;
	char buffer[BUFFER_SIZE];
	while (recv(connDescriptor, buffer, BUFFER_SIZE, MSG_PEEK | MSG_DONTWAIT) != -1) //read data without removing from queue
	{
		int bytesRead = FormPacket(buffer, BUFFER_SIZE);

		//consume [bytesRead] bytes from buffer
		recv(connDescriptor, buffer, bytesRead, 0);
	}
}
int RemotePlayer::FormPacket(char* buffer, int length)
{
	if (length < 1)
		return 0;

	int packetID = buffer[0];
	int bytesRead = 0;
	switch (packetID)
	{
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
void RemotePlayer::HandlePacket(PacketGameMove& packet)
{
	move = packet.GetMove();
	ready = true;
}
void RemotePlayer::HandlePacket(PacketCommand& packet)
{
	if (packet.GetCommand() == PacketCommand::COMMAND_PING)
		lastPong = time(nullptr);
}
void RemotePlayer::Send(const Packet& packet)
{
	const int bufferLength = Packet::MAX_PACKET_LENGTH;
	char buffer[bufferLength];
	int bytesWritten = packet.WriteToBuffer(buffer, bufferLength);
	send(connDescriptor, buffer, bytesWritten, 0);
}
bool RemotePlayer::IsConnected() const
{
	time_t currentTime = time(0);
	if (difftime(currentTime, lastPong) > CONNECTION_TIMEOUT)
		return false;
	return true;
}
void RemotePlayer::Think()
{
	ReceivePackets();
	if (difftime(time(nullptr), lastPing) > 1) //one second delay between pings to avoid flooding the connection
	{
		Send(PacketCommand(PacketCommand::COMMAND_PING));
		lastPing = time(nullptr);
	}
	if (!IsConnected())
	{
		GameEngine::GetInstance().ShowMainMenu();
		GameEngine::GetInstance().DisplayPopup("Disconnected");
		return;
	}
}
