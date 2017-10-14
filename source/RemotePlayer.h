#ifndef REMOTEPLAYER_H_
#define REMOTEPLAYER_H_
#include "Player.h"
#include "Network/Packet.h"
#include "Network/PacketGameMove.h"
#include "Network/PacketBoardData.h"
#include "Network/PacketCommand.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <iostream>
#include <ctime>
///Could not start server (Socket already bound or other network error)
class ServerException
{
};
/**
 * Remote Player class, works as a server, accepts connections done from RemoteGameMatch
 */
class RemotePlayer: public Player
{
public:
	RemotePlayer(const std::string& name);
	virtual ~RemotePlayer();

	///Is ready to provide the next move?
	virtual bool IsReady() const override;
	///Returns the move for this turn
	virtual GameMove GetMove() const override;

	///Called by engine to discard currently offered move (not valid move)
	virtual void RefuseMove() override;

	///Called when player's turn starts
	virtual void OnTurnBegin(const class GameMatch& match) override;

	///Send a packet to connected player
	void Send(const Packet& p);

	static const int LISTEN_PORT = 27015; ///< Port used for network connection

	///Think loop. Keeps connection alive and listens for packets.
	void Think();

	///Is the connection still alive
	bool IsConnected() const;
private:

	///Receive data from network
	void ReceivePackets();

	///Form packet from received data
	int FormPacket(char* buffer, int length);

	///Packet handler
	void HandlePacket(PacketGameMove& packet);

	///Packet handler
	void HandlePacket(PacketCommand& packet);


	addrinfo* addrInfo;
	bool ready;
	bool waitingForMove;
	int sock; ///<Socket to accept connections from
	int connDescriptor; ///<Socket for connection to client
	GameMove move; ///<Move received from client
	time_t lastPong; ///Time of the last response from the other side
	time_t lastPing; //Last time ping was send
	static const int CONNECTION_TIMEOUT = 10; ///<Connection is disconnected after time without activity
};

#endif /* REMOTEPLAYER_H_ */
