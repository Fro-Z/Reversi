#ifndef PACKETBOARDDATA_H_
#define PACKETBOARDDATA_H_
#include "Packet.h"
#include "../BoardData.h"
///Packet containing information about board tokens
class PacketBoardData: public Packet
{
public:
	PacketBoardData();
	PacketBoardData(const BoardData& board);
	virtual ~PacketBoardData();

	///Writes packet data to buffer, including packet ID
	virtual int WriteToBuffer(char* buffer, int length) const override;

	///Reads packet data from buffer, including packet ID
	virtual int ReadFromBuffer(char* buffer, int length) override;

	BoardData& GetBoard();

private:
	BoardData board;
};



#endif /* PACKETBOARDDATA_H_ */
