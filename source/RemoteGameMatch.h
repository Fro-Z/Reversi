#ifndef REMOTEGAMEMATCH_H_
#define REMOTEGAMEMATCH_H_
#include "GameMatch.h"
#include "Network/Packet.h"
#include "Network/PacketBoardData.h"
#include "Network/PacketGameMove.h"
#include "Network/PacketCommand.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
///Could not join match (No server response or network error)
class ClientException
{};
/**
 * @brief Remote Game Match class. Serves as a client for a server(Remote Player)
 *
 * Tasks:
 * -Update board each time new state is received.
 * -Can calculate new board state by itself as prediction, but correct server's state is used
 * -Redirect local player's moves to the server. Ask for a new move when move refused.
 *
 * Note: DummyPlayer class is used as second player.
 */
class RemoteGameMatch: public GameMatch
{
public:
	RemoteGameMatch(Player* localPlayer, const char* address, int port);
	virtual ~RemoteGameMatch();

	virtual void Think() override;

protected:
	///Send packet over network
	void Send(const Packet& packet);


	///Used as a remote player when first player instance is requested by GameEngine
	class DummyPlayer:public Player
	{
	public:
		DummyPlayer();
		virtual ~DummyPlayer();
		virtual GameMove GetMove() const override;
		virtual void RefuseMove() override;
		virtual void OnTurnBegin(const class GameMatch& match) override;
		virtual bool IsReady() const override;
	};
private:
	///Requests data from socket
	void ReceivePackets();

	///Forms packet based on ID in buffer
	int FormPacket(char* buffer, int length);

	///@{
	///Packet handler
	void HandlePacket(PacketBoardData& packet);
	void HandlePacket(PacketGameMove& packet);
	void HandlePacket(PacketCommand& packet);
	///@}

	bool waitingForRequest;
	addrinfo* addrInfo;
	int sock;

	time_t lastPong; //last response from the other side
	time_t lastPing; //last time ping was send
	static const int CONNECTION_TIMEOUT=10;
};



#endif /* REMOTEGAMEMATCH_H_ */
