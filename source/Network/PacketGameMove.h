#ifndef PACKETGAMEMOVE_H_
#define PACKETGAMEMOVE_H_
#include "Packet.h"
#include "../GameMove.h"
///Packet containing GameMove
class PacketGameMove: public Packet
{
public:
	PacketGameMove();
	PacketGameMove(const GameMove& move);
	virtual ~PacketGameMove();

	///Writes packet data to buffer, including packet ID
	virtual int WriteToBuffer(char* buffer, int length) const override;

	///Reads packet data from buffer, including packet ID
	virtual int ReadFromBuffer(char* buffer, int length) override;

	GameMove& GetMove();

private:
	GameMove move;
};



#endif /* PACKETGAMEMOVE_H_ */
